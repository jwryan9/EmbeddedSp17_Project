from __future__ import print_function
from datetime import date
import mlbgame
import csv

now = date.today()
year = now.year
month = now.month
day = now.day


def generate_score_data():
    games = mlbgame.games(year, month, day)
    comb_games = mlbgame.combine_games(games)

    # Write output file (format: away name, home name, inning, away score, home score, start time, AM/PM)
    # Inning format (game in progress: current inning, game complete: -1, game to be started: 0)
    with open('../Data/gameData.csv', 'wb') as csv_file:
        out_writer = csv.writer(csv_file)
        out_writer.writerow(['Away_Team', 'Home_Team', 'Inning', 'Away_Score', 'Home_Score', 'Start_Time', 'AM/PM'])
        for game in comb_games:
            overview = mlbgame.game.overview(game.game_id)

            if overview['status'] == 'Preview' or overview['status'] == 'Pre-Game':
                out_writer.writerow([overview['away_team_name'], overview['home_team_name'], '0', '0', '0',
                                     overview['home_time'], overview['home_ampm']])
            elif overview['status'] == 'Final':
                out_writer.writerow([overview['away_team_name'], overview['home_team_name'], '-1',
                                     overview['away_team_runs'], overview['home_team_runs'], overview['home_time'],
                                     overview['home_ampm']])
            elif overview['status'] == 'In Progress':
                out_writer.writerow([overview['away_team_name'], overview['home_team_name'], overview['inning'],
                                     overview['away_team_runs'], overview['home_team_runs'], overview['home_time'],
                                     overview['home_ampm']])
    print('Game Data Generated')
