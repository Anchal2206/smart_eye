import requests

esp32_cam_url = "http://192.168.43.42/photo"
image_path = "static/captured.jpg"

# Fetch the image from ESP32-CAM
response = requests.get(esp32_cam_url)

if response.status_code == 200:
    with open(image_path, "wb") as f:
        f.write(response.content)
    print("Image saved successfully.")
else:
    print("Failed to fetch image.")

