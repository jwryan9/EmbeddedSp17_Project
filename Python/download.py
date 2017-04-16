import urllib
from datetime import date

currDate = date.today()
year = currDate.year
month = currDate.month
day = currDate.day

yearSring = 'year_%d' % year

if month < 10:
    monthString = 'month_0%d' % month
else:
    monthString = 'month_%d' % month

if day < 10:
    dayString = 'day_0%d' % day
else:
    dayString = 'day_%d' % day

dateString = yearSring + '/' + monthString + '/' + dayString + '/'
URL_String = 'http://gd2.mlb.com/components/game/mlb/' + dateString + '/master_scoreboard.json'

mlbData = urllib.URLopener()
mlbData.retrieve(URL_String, '/Users/jasonryan/PycharmProjects/EmbeddedSp17_Project/Data/master_scoreboard.json')
