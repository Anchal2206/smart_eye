from flask import Flask, request
import os

app = Flask(__name__)

@app.route("/", methods=["GET"])
def home():
    return "<h2> Flask server is running! Use /trigger endpoint to POST from ESP32.</h2>"

@app.route("/trigger", methods=["POST"])
def handle_trigger():
    print(" Trigger received from ESP32")
    print(" saving image...")
    os.system("python image_extraction.py")
    print(" generating output...")
    os.system("python ai_integration.py")
    print(" generating audio...")
    os.system("python speach.py")
    return "AI Processing Complete!", 200

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)


