import requests
import urllib.request
import time
from bs4 import BeautifulSoup

url = 'https://www.bbc.co.uk/weather/2643743'
response = requests.get(url)
print(response)

soup = BeautifulSoup(response.text, 'html.parser')
#print(soup.get_text())
data = soup.findAll('li')

#for i,d in enumerate(data):print(i, d)

now = data[32]
next_hour =  data[33]
# and so on

divs = now.findAll('div')

#for i, d in enumerate(divs):print(i, d)

time = divs[3].get_text()
weather = divs[5]
print()
print(time)
print()
w_divs = weather.findAll('div')

for i, w in enumerate(w_divs):
    print(i, w.get_text())
    print()
weather = w_divs[0].get_text()

temp = w_divs[7].get_text()[0:2]

precip = w_divs[8].get_text()[0:2]

wind_speed = w_divs[11].get_text()[16:21]
wind_direction = w_divs[11].get_text()[-19:]


print(time, weather, temp, '. Precipiation: ', precip, '. Wind speed: ', wind_speed, '. Dir: ',  wind_direction)
