from __future__ import print_function
from datetime import date
import mlbgame
import csv

now = date.today()
year = now.year
month = now.month
day = now.day

games = mlbgame.games(year, month, day)
combGames = mlbgame.combine_games(games)


# Write output file (format: away name, home name, inning, away score, home score, start time)
# Inning format (game in progress: current inning, game complete: -1, game to be started: 0)
with open('gameData.csv', 'wb') as csv_file:
    out_writer = csv.writer(csv_file)
    for game in combGames:
        overview = mlbgame.game.overview(game.game_id)

        if overview['status'] == 'Preview' or overview['status'] == 'Pre-Game':
            out_writer.writerow([overview['away_team_name'], overview['home_team_name'], '0', '0', '0',
                                 overview['home_time']])
        elif overview['status'] == 'Final':
            out_writer.writerow([overview['away_team_name'], overview['home_team_name'], '-1',
                                 overview['away_team_score'], overview['home_team_score'], overview['home_time']])
        elif overview['status'] == 'In Progress':
            out_writer.writerow([overview['away_team_name'], overview['home_team_name'], overview['inning'],
                                 overview['away_team_runs'], overview['home_team_runs'], overview['home_time']])
