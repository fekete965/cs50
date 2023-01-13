import csv
import sys


def main():
    # Check for command-line usage
    if (len(sys.argv) < 3):
        print("Missing command-line argument(s)")
        sys.exit(1)

    dbPath = sys.argv[1]
    sequencePath = sys.argv[2]
    
    # Read database file into a variable
    db = readDb(dbPath)
    
    # Read DNA sequence file into a variable
    sequence = readSequence(sequencePath)

    # Find longest match of each STR in DNA sequence and mark potential matches
    possibleMatches = markPotentialMatches(db, sequence)

    # Find the match if there is any
    result = getMatch(possibleMatches)

    print(result)

    return sys.exit(0)


def getMatch(possibleMatches: dict[str: dict[str: bool]]):
    for name in possibleMatches:
        matchValuList = list(possibleMatches[name].values())
        if (matchValuList).count(True) == len(matchValuList):
            return name

    return "No match"


def markPotentialMatches(db: dict[str: dict[str: int]], sequence: str):
    sequenceDataDict = {}

    for name in db:
        shortTandemRepeats = db[name]
        sequenceDataDict[name] = {}

        for subsequence in shortTandemRepeats:
            subsequenceCount = longest_match(sequence, subsequence)

            if (shortTandemRepeats[subsequence] != subsequenceCount):
                sequenceDataDict[name][subsequence] = False
            else:
                sequenceDataDict[name][subsequence] = True
    
    return sequenceDataDict


def readDb(path: str):
    dbDict = {}
    
    with open(path, "r") as file:
        reader = csv.DictReader(file)

        for row in reader:
            name = row["name"]
            row.pop("name")

            itemPairs = row.items()
            itemDict = [marshalTuples(x) for x in itemPairs]

            dbDict[name] = dict(itemDict)

    return dbDict


def marshalTuples(valuePair: tuple[str, str]):
    [shortTandemRepeats, count] = valuePair
    return tuple([shortTandemRepeats, int(count)])


def readSequence(path: str):
    with open(path, "r") as file:
        return file.read()


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1
            
            # If there is no match in the substring
            else:
                break
        
        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
