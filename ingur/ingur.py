import configparser
from imgurpython import ImgurClient

config = configparser.ConfigParser()
config.read('auth.ini')

client_id = config.get('credentials', 'client_id')
client_secret = config.get('credentials', 'client_secret')

client = ImgurClient(client_id, client_secret)

items = client.gallery()    
for item in items:
    print(item.link)
    print(item.title)
    
    
    
