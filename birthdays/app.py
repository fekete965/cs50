import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")

# Constant field names
DAY = 'day'
ID = 'id'
MONTH = 'month'
NAME = 'name'


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"

    return response


def getBirthdays():
    birthdays = db.execute("SELECT * FROM birthdays ORDER BY name ASC;")
    return birthdays


def findBirthday(id):
    birthday = db.execute("SELECT * FROM birthdays WHERE id = ? LIMIT 1;", id)

    if len(birthday) == 0:
        return (None, "Cannot find birthday!")

    return (birthday[0], None)


def addBirthday(formFields):
    name = formFields[NAME]
    month = formFields[MONTH]
    day = formFields[DAY]

    db.execute("INSERT INTO birthdays (name, month, day) VALUES (?, ?, ?)", name, month, day)


def updateBirthday(id, formFields):
    name = formFields[NAME]
    month = formFields[MONTH]
    day = formFields[DAY]

    db.execute("UPDATE birthdays SET name = ?, month = ?, day = ? WHERE id = ?;", name, month, day, id)


def getFormFields(requestForm):
    requiredFields = {}
    requiredFields[DAY] = request.form.get(DAY)
    requiredFields[MONTH] = request.form.get(MONTH)
    requiredFields[NAME] = request.form.get(NAME)

    return requiredFields

def validateFields(fieldDictionary):
    for field in fieldDictionary:
        fieldValue = fieldDictionary[field]

        if not fieldValue:
            return f"{field.capitalize()} is required"

    return None

@app.route("/", methods=["GET", "POST"])
def index():
    birthdays = getBirthdays()

    if request.method == "POST":
        idOpt = request.form.get(ID)

        formFields = getFormFields(request.form)
        formError = validateFields(formFields)
        if formError:
            return render_template("index.html", birthdays=birthdays, formError=formError, targetId=None)

        if (idOpt):
            updateBirthday(idOpt, formFields)
        else:
            addBirthday(formFields)

        return redirect("/")

    else:
        return render_template("index.html", birthday=None, birthdays=birthdays, targetId=None)


@app.route("/delete", methods=["POST"])
def delete():
    id = request.form.get(ID)
    if (id):
        db.execute("DELETE FROM birthdays WHERE id = ?", id)
    
    return redirect("/")


@app.route("/cancel-editing", methods=["GET", "POST"])
def cancelEditing():
    return redirect("/")

@app.route("/edit/<birthdayId>", methods=["POST"])
def edit(birthdayId):
    birthdays = getBirthdays()
    (birthday, formError) = findBirthday(birthdayId)

    return render_template("index.html", birthday=birthday, birthdays=birthdays, formError=formError)
