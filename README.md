# Assistive Vision Device  
An AI-integrated embedded system for visually impaired assistance — capturing an image via onboard camera, generating scene descriptions using Gemini, and converting it into speech using TTS models.

---

## Overview

This project presents a portable assistive device aimed at helping visually impaired individuals interpret their surroundings. Using a 2MP camera and embedded AI modules, the device captures real-world images, generates meaningful textual descriptions using a Language Model (LLM), and reads them aloud through a speaker using a Text-to-Speech (TTS) engine.

---

## Components

### Hardware
- 2MP USB Camera Module  
- Arduino Nano 
- Speaker + Audio Driver Circuit  
- Power Supply (Battery)  

### Software
- Python (backend + AI pipeline)
- Arduino IDE (hardware control)
- Gemini API (for text generation)
- pyttsx3 (TTS engine)

---

## Features

- Capture image via physical trigger
- AI caption generation using Vision-to-Text model
- Text-to-Speech conversion for scene narration
- Serial communication with Arduino for status and audio triggering

---

## How It Works

1. **User triggers the camera** (either manually or via motion sensor).
2. `image_extraction.py` captures and saves the image.
3. `ai_integration.py` sends the image to a cloud-hosted LLM (i.e., Gemini-1.5-pro) and receives a descriptive caption.
4. `speach.py` converts the caption to speech using TTS (i.e., pyttsx3).
5. `PROTO1.ino` on Arduino receives command via serial and plays audio output.
6. Complete feedback loop ensures modularity and reusability of each block.

