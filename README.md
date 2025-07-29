# Assistive Vision Device  
An AI-integrated embedded system for visually impaired assistance — capturing an image via onboard camera, generating scene descriptions using an LLM, and converting it into speech using TTS models.

---

## Overview

This project presents a portable assistive device aimed at helping visually impaired individuals interpret their surroundings. Using a 2MP camera and embedded AI modules, the device captures real-world images, generates meaningful textual descriptions using a Language Model (LLM), and reads them aloud through a speaker using a Text-to-Speech (TTS) engine.

---

## System Architecture
[Camera] → [Python Server]
↳ image_extraction.py
↳ ai_integration.py
↳ speach.py
↓
[LLM (Image Captioning)] → Text Description
↓
[TTS Engine] → Speech Output
↓
[Speaker (via Arduino Serial Interface)]

---

## Components

### Hardware
- 2MP USB Camera Module  
- Arduino Nano (or compatible microcontroller)  
- Speaker + Audio Driver Circuit  
- Power Supply (Battery or USB)  

### Software
- Python (backend + AI pipeline)
- Arduino IDE (hardware control)
- OpenAI/Gemini API (for text generation)
- gTTS or pyttsx3 (TTS engine)
- Serial Communication using `pyserial`

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
3. `ai_integration.py` sends the image to a cloud-hosted LLM (e.g., Gemini or GPT-4V) and receives a descriptive caption.
4. `speach.py` converts the caption to speech using TTS (e.g., gTTS).
5. `PROTO1.ino` on Arduino receives command via serial and plays audio output.
6. Complete feedback loop ensures modularity and reusability of each block.

