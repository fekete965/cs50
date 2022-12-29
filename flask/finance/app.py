import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, formatName, getTimeStamp, login_required, lookup, usd, validatePassword

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd
app.jinja_env.filters["formatName"] = formatName

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")

# User actions for user shares table 
BUY = "BUY"
SELL = "SELL"


def getGroupedUserSharesByUserAction(userId, userAction):
    return db.execute("""
        SELECT 
            us.name,
            us.price AS exchanged_price,
            us.symbol,
            SUM(us.shares) AS shares,
            SUM(us.total_value) AS exchanged_total_share_value,
            us.user_action
        FROM 
            users u
        JOIN 
            user_shares us
        ON 
            us.user_id = u.id
        WHERE 
            u.id = ?
            AND us.user_action = ?
        GROUP BY 
            us.symbol;
    """, userId, userAction)


def getUserShares(userId):
    soldShares = getGroupedUserSharesByUserAction(userId, SELL)
    boughtShares = getGroupedUserSharesByUserAction(userId, BUY)

    totalShares = boughtShares + soldShares

    sharesDict = {}

    for share in totalShares:
        name = share.get("name")
        exchangedPrice = share.get("exchanged_price")
        symbol = share.get("symbol")
        userAction = share.get("user_action")
            
        exchangedTotalShareValue = share.get("exchanged_total_share_value")
        shares = share.get("shares")
        if userAction == SELL:
            shares *= -1
            exchangedTotalShareValue *= -1

        if not symbol in sharesDict:
            sharesDict[symbol] = {
                "name": name,
                "exchangedPrice": exchangedPrice,
                "symbol": symbol,
                "shares": shares,
                "exchangedTotalShareValue": exchangedTotalShareValue,
            }
        else:
            sharesDict[symbol]["shares"] += shares
            sharesDict[symbol]["exchangedTotalShareValue"] += exchangedTotalShareValue

    return list(sharesDict.values())


def getUserSharesByUserAction(userId, userAction):
    return db.execute("""
        SELECT 
            *
        FROM 
            users u
        JOIN 
            user_shares us
        ON 
            us.user_id = u.id
        WHERE 
            u.id = ?
            AND us.user_action = ?;
    """, userId, userAction)


def getUserSharesTotalByAction(userId, userAction):
    return db.execute("""
        SELECT 
            SUM(us.total_value) AS total_value
        FROM 
            users u
        JOIN 
            user_shares us
        ON 
            us.user_id = u.id
        WHERE
            u.id = ?
            AND us.user_action = ?;
    """, userId, userAction)


def getUserSymbolAndShareDataByUserAction(userId, userAction):
    return db.execute("""
        SELECT
            us.symbol,
            SUM(us.shares) AS total_shares,
            us.user_action
        FROM
            users u
        JOIN 
            user_shares us
        ON 
            u.id = us.user_id
        WHERE 
            u.id = ?
            AND us.user_action = ?
        GROUP BY
            us.symbol
    """, userId, userAction)


def getUserSymbolAndShareData(userId):
    sold = getUserSymbolAndShareDataByUserAction(userId, SELL)
    bought = getUserSymbolAndShareDataByUserAction(userId, BUY)

    allData = sold + bought

    symbolDataDict = {}
    for data in allData:
        symbol = data.get("symbol")
        userAction = data.get("user_action")

        totalShares = data.get("total_shares")
        if userAction == SELL:
            totalShares *= -1

        if not symbol in symbolDataDict:
            symbolDataDict[symbol] = {
                "symbol": symbol,
                "totalShares": totalShares
            }
        else:
            symbolDataDict[symbol]["totalShares"] += totalShares

    symbolList = list(symbolDataDict.values())
    filteredList = list(filter(lambda i: i.get("totalShares") != 0, symbolList))
    print(filteredList)

    return filteredList


def getFreshShareData(symbolList):
    freshShareData = {}
    for symbol in symbolList:
        data = lookup(symbol)
        freshShareData[symbol] = data.get("price", 0)

    return freshShareData


def setCurrentPrices(freshShareData):
    def helper(shareData):
        symbol = shareData.get("symbol")
        shareCount = shareData.get("shares")

        currentPrice = freshShareData.get(symbol, 0)
        shareData["currentPrice"] = currentPrice
        shareData["currentTotalShareValue"] = currentPrice * shareCount

        return shareData
    return helper


def getUserShareHistory(userId):
    return db.execute("""
        SELECT
            us.name,
            us.user_action AS userAction,
            us.price,
            us.symbol,
            us.shares,
            us.total_value AS totalValue,
            us.created_at AS dateTime
        FROM
            users u
        JOIN 
            user_shares us
        ON 
            u.id = us.user_id
        ORDER BY
            us.id DESC;
    """)


def getUserBalance(userId):
    currentUserCredits = db.execute("SELECT cash FROM users u WHERE u.id = ? LIMIT 1;", userId)
    if len(currentUserCredits) == 0:
        return float(0)

    return float(currentUserCredits[0].get("cash"))


def addCreditToUser(userId, credit):
    userCurrentBalance = getUserBalance(userId)
    newCredit = userCurrentBalance + credit

    return db.execute("""
        UPDATE
            users
        SET
            cash = ?
        WHERE
            id = ?;
    """, newCredit, userId)


def updatePassword(userId, newPassword):
    hash = generate_password_hash(newPassword)

    return db.execute("""
        UPDATE
            users
        SET
            hash = ?
        WHERE 
            id = ?
    """, hash, userId)


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    userId = session.get("user_id")
    userShares = getUserShares(userId)
    
    symbolList = list(map(lambda us: us["symbol"], userShares))
    
    userShares = filter(lambda us: us["shares"] != 0, userShares)
    
    freshShareData = getFreshShareData(symbolList)
    userShares = list(map(setCurrentPrices(freshShareData), userShares))

    userSharesSummary = {}
    userSharesSummary["shares"] = userShares
    userSharesSummary["exchangedTotalValue"] = 0
    userSharesSummary["currentTotalValue"] = 0

    currentShareValues = sum(list(map(lambda share: share.get("currentTotalShareValue", 0), userShares)))
    userSharesSummary["currentTotalValue"] = currentShareValues

    userSharesTotalValue = sum(list(map(lambda share: share.get("exchangedTotalShareValue", 0), userShares)))
    userSharesSummary["exchangedTotalValue"] = userSharesTotalValue

    return render_template("portfolio.html", userSharesSummary=userSharesSummary)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "GET":
        return render_template("buy.html")

    symbol = request.form.get("symbol")
    shares = request.form.get("shares")
    if not symbol:
        flash("Symbol field is required")
        return render_template("buy.html")
    if not shares:
        flash("Shares field is required")
        return render_template("buy.html")

    data = lookup(symbol)

    if not data:
        flash("Symbol cannot be found")
        return render_template("buy.html")

    userId = session.get("user_id")
    users = db.execute("SELECT * FROM users WHERE id = ? LIMIT 1;", userId)

    totalValue = data.get("price", 0) * int(shares)

    user = users[0]
    userCredit = user.get("cash")
    if userCredit < totalValue:
        flash(f"You don't have enough credit to buy that much shares.\nYou have {userCredit}/{totalValue} credits\n")
        return render_template("buy.html")

    deductedCash = userCredit - totalValue
    db.execute("UPDATE users SET cash = ? WHERE id = ?;", deductedCash, userId)

    name = data.get("name")
    price = data.get("price")
    symbol = data.get("symbol")
    now = getTimeStamp()
    db.execute("""
        INSERT INTO
            user_shares (user_id, name, user_action, price, symbol, shares, total_value, created_at) 
        VALUES 
            (?, ?, ?, ?, ?, ?, ?, ?);""",
                userId, name, BUY, price, symbol, shares, totalValue, now)

    return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    
    userId = session.get("user_id")
    historyData = getUserShareHistory(userId)

    return render_template("history.html", historyData=historyData)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote.html")

    symbol = request.form.get("symbol")
    if not symbol:
        flash("Symbol field is required")
        return render_template("quote.html")

    data = lookup(symbol)
    print(data)
    if not data:
        flash("Data cannot be found")
        return render_template("quote.html")
    
    return render_template("quoted.html", data=data)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "GET":
        return render_template("register.html")

    username = request.form.get('username')
    password = request.form.get('password')
    repeatedPassword = request.form.get('repeatedPassword')

    passwordErrorMsg = validatePassword(password, repeatedPassword)
    if passwordErrorMsg:
        flash(passwordErrorMsg)
        return render_template("register.html")

    user = db.execute("SELECT * FROM users WHERE username = ? LIMIT 1;", username)

    if len(user) != 0:
        flash("Username is already in use. Pick another one.")
        return render_template("register.html")

    hashedPassword = generate_password_hash(password)

    db.execute("INSERT INTO users (username, hash) VALUES (?, ?);", username, hashedPassword)

    return redirect('/login')


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    userId = session.get("user_id")
    symbolAndShareData = getUserSymbolAndShareData(userId)
    
    if request.method == "GET":
        return render_template("sell.html", symbolAndShareData=symbolAndShareData)

    selectedSymbol = request.form.get("symbol")
    sharesStr = request.form.get("shares")

    if not selectedSymbol:
        flash("Please select a share.")
        return render_template("sell.html", symbolAndShareData=symbolAndShareData)
    if not sharesStr:
        flash("Please define the amount of shares you want to sell.")
        return render_template("sell.html", symbolAndShareData=symbolAndShareData)
    
    shares = float(sharesStr)
    selectedSymbolData = next((item for item in symbolAndShareData if item["symbol"] == selectedSymbol), None)

    if not selectedSymbolData:
        flash("Invalid selected share.")
        return render_template("sell.html", symbolAndShareData=symbolAndShareData)

    if shares < 0:
        flash("Share amount has to be a positive integer.")
        return render_template("sell.html", symbolAndShareData=symbolAndShareData)

    if shares > float(selectedSymbolData.get("totalShares")):
        flash("Cannot sell more share that you own.")
        return render_template("sell.html", symbolAndShareData=symbolAndShareData)

    freshShareData = lookup(selectedSymbol)
    selectedSharePrice = float(freshShareData.get("price", 0))
    if (selectedSharePrice == 0):
        flash("Something went wrong and couldn't find the selected share data.")
        return render_template("sell.html", symbolAndShareData=symbolAndShareData)

    totalCredits = selectedSharePrice * shares

    userCurrentBalance = userCurrentBalance = getUserBalance(userId)
    newUserCreditAmount = totalCredits + userCurrentBalance

    db.execute("""
        UPDATE
            users
        SET
            cash = ?
        WHERE
            id = ?;
    """, newUserCreditAmount, userId)

    name = freshShareData.get("name")
    price = freshShareData.get("price")
    symbol = freshShareData.get("symbol")
    now = getTimeStamp()
    db.execute("""
        INSERT INTO
            user_shares 
            (user_id, name, user_action, price, symbol, shares, total_value, created_at) 
        VALUES 
            (?, ?, ?, ?, ?, ?, ?, ?);
    """,
                userId, name, SELL, price, symbol, shares, totalCredits, now)

    return redirect("/")


@app.route("/account")
@login_required
def account():
    """Getting account information"""

    userId = session.get("user_id")
    currentBalance = getUserBalance(userId)

    return render_template("account.html", currentBalance=currentBalance)


@app.route("/account/add-credit", methods=["POST"])
@login_required
def addCredit():
    """Add credit to user account"""

    creditStr = request.form.get("credit")

    if not creditStr:
        flash("Credit amount is required.")
        return redirect("/account")

    credit = float(creditStr)
    if credit == 0:
        flash("Credit amount has to be a positive integer.")
        return redirect("/account")

    userId = session.get("user_id")
    addCreditToUser(userId, credit)
    
    flash("Credits have been added.")
    return redirect("/account")


@app.route("/account/change-password", methods=["POST"])
@login_required
def changePassword():
    """Update user's password"""

    password = request.form.get('password')
    repeatedPassword = request.form.get('repeatedPassword')

    passwordErrorMsg = validatePassword(password, repeatedPassword)
    if passwordErrorMsg:
        flash(passwordErrorMsg)
        return redirect("/account")

    userId = session.get("user_id")
    updatePassword(userId, password)

    flash("Password has been updated.")
    return redirect("/account")
