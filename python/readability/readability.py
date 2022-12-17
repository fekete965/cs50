from cs50 import get_string
from math import ceil

# Define constants
DOT = 46
EXCLAMATION_MARK = 33
QUESTION_MARK = 63
SENTENCE_STOPS = set([DOT, EXCLAMATION_MARK, QUESTION_MARK])
LOWERCASE_A = 97
LOWERCASE_Z = 122
NUL = 0
WHITE_SPACE = 32


def main():
  text = get_string("Text: ")

  # Gather text data
  [letters, words, sentences] = gatherData(text)

  # Calculate the book's grade
  # L is the average number of letters per 100 words
  L = letters / words * 100

  # S is the average number of sentences per 100 words
  S = sentences / words * 100
  grade = 0.0588 * L - 0.296 * S - 15.8

  # Print out the grade accordingly
  if (grade < 1.0):
    print("Before Grade 1")
  elif (grade >= 16):
    print("Grade 16+")
  else:
    print(f"Grade: {ceil(grade)}")


# Define a function which counts the letters, words and senteces in a given text
def gatherData(text: str):
  letters = 0
  words = 0
  sentences = 0

  for char in text:
    charCode = ord(char.lower())
    # Check for WHITE_SPACE
    if (charCode == WHITE_SPACE):
      # Increase word count
      words += 1
    # Check for DOT, EXCLAMATION_MARK and QUESTION_MARK
    elif (charCode in SENTENCE_STOPS):
      sentences += 1
    # Check if character is a-zA-Z
    elif (charCode >= LOWERCASE_A and charCode <= LOWERCASE_Z):
      letters += 1

  # Increase the word count if we reached the end of the text
  words += 1

  return tuple([letters, words, sentences])


main()
