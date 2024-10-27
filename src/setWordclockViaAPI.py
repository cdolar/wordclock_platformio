#! /usr/bin/env python

from datetime import datetime
import requests


url = datetime.now().strftime("http://wordclock.local/api/setDateTime?day=%d&month=%m&year=%Y&hour=%H&minute=%M")
print("Trying to set wordclock with URL: {}".format(url))
response = requests.get(url)
print(response)