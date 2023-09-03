

from imgurpython import ImgurClient
import configparser

def authenticate():
    config = configparser.ConfigParser()
    config.read('auth.ini')

    client_id = config.get('credentials', 'client_id')
    client_secret = config.get('credentials', 'client_secret')
    imgur_username = config.get('credentials', 'imgur_username')
    imgur_password = config.get('credentials', 'imgur_password')

    client = ImgurClient(client_id, client_secret)

    authorization_url = client.get_auth_url('pin')

    print(authorization_url)

authenticate()