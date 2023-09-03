import os
from imgurpython import ImgurClient


def upload_image(image_path):
    # Path to the local image file you want to upload
    # image_path = "C:\\Users\\Saskarkhadka\\OneDrive\\Desktop\\next.jpg"
    # Replace with your own Imgur API credentials
    client_id = "8de103f6c4335f4"
    client_secret = "b7e1a5a492bcbfc03ebe0aa2e647a67a80166ec3"

    # Initialize the Imgur client
    client = ImgurClient(client_id, client_secret)
    # Check if the image file exists
    if not os.path.isfile(image_path):
        print("Image file does not exist.")
        return None
    else:
        # Upload the image to Imgur
        try:
            uploaded_image = client.upload_from_path(image_path, config=None, anon=True)
            print("Image uploaded successfully.")
            print("Image URL: ", uploaded_image["link"])
            return uploaded_image["link"]
        except Exception as e:
            print("An error occurred while uploading the image:", str(e))
            return None


# print(
#     upload_image(
#         "C:\\Users\\Saskarkhadka\\OneDrive\\Pictures\\vlcsnap-2021-03-26-15h32m47s812.png"
#     )
# )
