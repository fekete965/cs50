import os
import requests
import urllib.parse

from datetime import datetime

from flask import redirect, render_template, request, session
from functools import wraps
from string import punctuation

MIN_PASSWORD_LENGTH = 6


def apology(message, code=400):
    """Render message as an apology to user."""
    def escape(s):
        """
        Escape special characters.

        https://github.com/jacebrowning/memegen#special-characters
        """
        for old, new in [("-", "--"), (" ", "-"), ("_", "__"), ("?", "~q"),
                         ("%", "~p"), ("#", "~h"), ("/", "~s"), ("\"", "''")]:
            s = s.replace(old, new)
        return s
    return render_template("apology.html", top=code, bottom=escape(message)), code


def login_required(f):
    """
    Decorate routes to require login.

    http://flask.pocoo.org/docs/0.12/patterns/viewdecorators/
    """
    @wraps(f)
    def decorated_function(*args, **kwargs):
        if session.get("user_id") is None:
            return redirect("/login")
        return f(*args, **kwargs)
    return decorated_function


def lookup(symbol):
    """Look up quote for symbol."""

    # Contact API
    try:
        api_key = os.environ.get("API_KEY")
        url = f"https://api.iex.cloud/v1/data/core/quote/{urllib.parse.quote_plus(symbol)}?token={api_key}"
        response = requests.get(url)
        response.raise_for_status()
    except requests.RequestException:
        return None

    # Parse response
    try:
        quote = response.json()[0]
        return {
            "name": quote["companyName"],
            "price": float(quote["latestPrice"]),
            "symbol": quote["symbol"]
        }
    except (KeyError, TypeError, ValueError):
        return None


def usd(value):
    """Format value as USD."""
    return f"${value:,.2f}"


def getTimeStamp():
    now = datetime.now()
    return now.strftime("%d/%m/%Y %H:%M:%S")


def formatName(name: str):
    cleanedName = name.strip()
    if cleanedName == '':
        return 'N/A'

    return cleanedName
    

def hasUpper(string):
    return any(c.isupper() for c in string)


def hasDigit(string):
    return any(c.isdigit() for c in string)


def hasSymbol(string):
    punctuations = set(punctuation)
    return any(c in punctuations for c in string)


def isPasswordApplicable(password):
    """Password has to contain:
        - at least 1 uppercase character
        - at least 1 digit
        - at least 1 special character
    """

    return hasUpper(password) and hasDigit(password) and hasSymbol(password)

def validatePassword(password, repeatedPassword):
    if not password:
        return "Password field is required!"
    if len(password) < MIN_PASSWORD_LENGTH:
        return f"Password has to be at least {MIN_PASSWORD_LENGTH} long!"
    if not repeatedPassword:
        return "Repeat your password please!"
    if password != repeatedPassword:
        return "The passwords aren't match."
    if not isPasswordApplicable(password):
        return "The has to contain at least 1 uppercase character, a number and a speacial character."

    return None
