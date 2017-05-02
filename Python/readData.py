import pandas as pd


def read_data(file_path):
    df = pd.read_csv(file_path)
    away_teams = df['Away_Team'].astype('str')
    home_teams = df['Home_Team'].astype('str')
    innings = df['Inning'].astype('str')
    away_scores = df['Away_Score'].astype('str')
    home_scores = df['Home_Score'].astype('str')
    start_times = df['Start_Time'].astype('str')
    am_pms = df['AM/PM'].astype('str')

    num_games = len(df.index)
    out_str = ''

    for i in range(num_games):
        out_str += away_teams[i] + ',' + home_teams[i] + ',' + innings[i] + ',' + away_scores[i] + ',' + home_scores[i] \
                  + ',' + start_times[i] + ',' + am_pms[i] + ';'

    return out_str