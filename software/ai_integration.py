import google.generativeai as genai
from PIL import Image
import os

# Configure API
genai.configure(api_key="AIzaSyCeuygNDBe9itoTq_KVud0jyaRO0XdqF2Y")
model = genai.GenerativeModel("gemini-1.5-pro")

# Load image
img = Image.open("static/captured.jpg")


#Description
prompt="I am a ai vision camera specifically design to help blind people identify and understand the environment. When image is provided look at the image and explain everything happening in the image to the user, if there is a person in the image  tell his/her appearance, if there is book or any text help the user to understand whats written. Start your description like a human will say"
# Generate description
response = model.generate_content([prompt, img])

# Ensure 'static' folder exists
os.makedirs("static", exist_ok=True)

# Save description in static folder
with open("static/description.txt", "w") as f:
    f.write(response.text)

print("✅ AI description saved to static/description.txt")
