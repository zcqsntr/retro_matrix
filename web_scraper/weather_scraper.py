import requests
import subprocess
import time
from bs4 import BeautifulSoup
import time



url = 'https://www.bbc.co.uk/weather/2643743'
response = requests.get(url)
print(response)

soup = BeautifulSoup(response.text, 'html.parser')
#print(soup.get_text())
data = soup.findAll('li')

#for i,d in enumerate(data):print(i, d)

now = data[33]
next_hour =  data[33]
# and so on

divs = now.findAll('div')

#for i, d in enumerate(divs):print(i, d)

time = divs[3].get_text()
weather = divs[5]

w_divs = weather.findAll('div')


weather = w_divs[0].get_text()

temp = w_divs[7].get_text()[0:2]

precip = w_divs[8].get_text()[0:2]

wind_speed = w_divs[11].get_text()[16:21]
wind_direction = w_divs[11].get_text()[-20:]


print(time, weather, temp, '. Precipiation: ', precip, '. Wind speed: ', wind_speed, '. Dir: ',  wind_direction)

message = time 

for m in [weather, temp, precip, wind_speed, wind_direction]:
    message += ',' + m.replace(" ", "").replace("/n", "").lower()
message+="/n"

print('Message sent to C++', message)

proc = subprocess.Popen(["./display_weather", "--led-slowdown-gpio=4"],stdin=subprocess.PIPE)
proc.stdin.write(message.encode('utf-8'))

cppMessage = proc.stdout.readline().rstrip("\n") 
print("cppreturn message ->" + cppMessage + " written by python \n")


time.sleep(10)
proc.kill()    
# wethaer types 
#   sunny 
#   sunnyintervals

# directions 
#   southsoutheasterly 
#   northwesterly
