import csv
from cs50 import get_string

name = get_string("Name: ")
number = get_string("Number: ")

with open("phonebook.csv", "a") as file:
  writer = csv.writer(file)
  writer.writerow([name, number])
