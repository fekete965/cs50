# Simulate a sports tournament

import csv
import sys
import random

# Number of simluations to run
N = 1000


def main():

    # Ensure correct usage
    if len(sys.argv) != 2:
        sys.exit("Usage: python tournament.py FILENAME")

    teams = []
    # Read teams into memory from file
    teamsPath = sys.argv[1]
    with open(teamsPath, "r") as file:
        reader = csv.DictReader(file)

        for row in reader:
            teamDict = {
                "team": row["team"],
                "rating": int(row["rating"])
            }
            teams.append(teamDict)

    counts = {}
    # Simulate N tournaments and keep track of win counts
    for _ in range(0, N):
        winnerName = simulate_tournament(teams)
        
        if counts.get(winnerName) != None:
            counts[winnerName] += 1
        else:
            counts.update({winnerName: 1})

    # Print each team's chances of winning, according to simulation
    for team in sorted(counts, key=lambda team: counts[team], reverse=True):
        print(f"{team}: {counts[team] * 100 / N:.1f}% chance of winning")


def simulate_game(team1: dict, team2: dict):
    """Simulate a game. Return True if team1 wins, False otherwise."""
    rating1 = team1["rating"]
    rating2 = team2["rating"]
    probability = 1 / (1 + 10 ** ((rating2 - rating1) / 600))
    return random.random() < probability


def simulate_round(teams: list[dict]):
    """Simulate a round. Return a list of winning teams."""
    winners = []

    # Simulate games for all pairs of teams
    for i in range(0, len(teams), 2):
        if simulate_game(teams[i], teams[i + 1]):
            winners.append(teams[i])
        else:
            winners.append(teams[i + 1])

    return winners


def simulate_tournament(teams: list[dict]):
    """Simulate a tournament. Return name of winning team."""

    def helper(teams: list[dict]):
        if (len(teams) == 1):
            return teams

        roundWinners = simulate_round(teams)
        return helper(roundWinners)
    
    return helper(teams)[0].get("team")


if __name__ == "__main__":
    main()
