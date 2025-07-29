import pyttsx3

# Initialize TTS engine
engine = pyttsx3.init()

# Read the text
with open("static/description.txt", "r") as f:
    text = f.read()

# Optional: adjust voice rate or volume
engine.setProperty('rate', 150)      # Speed (default is ~200)
engine.setProperty('volume', 1.0)    # Volume (0.0 to 1.0)

# Speak the text
engine.say(text)
engine.runAndWait()
