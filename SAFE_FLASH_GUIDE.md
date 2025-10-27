⏺ Great question! Let me explain the safe flashing process for your DeskHog board:

  🔒 Safe Flashing Guide

  Will Flashing Delete Your Data?

  Short answer: NO! Your data is safe because:

  1. Persistent storage is separate - WiFi credentials, card configurations, and
  Navagotchi state are stored in NVS (Non-Volatile Storage), which is in a different
  partition from the firmware
  2. Dual OTA partitions - The board uses two app partitions, so updates toggle between
  them without touching your data

  However, There's One Case Where Data Gets Erased:

  "Erase Flash and Upload" (from PlatformIO menu) - This wipes EVERYTHING including your
   settings. Only use this if:
  - The board won't boot
  - You're stuck in a boot loop
  - You need a completely fresh start

  ---
  📝 Safe Flashing Process

  Step 1: Connect the Board

  1. Connect via USB-C to your computer
  2. The board should appear as a serial device

  Step 2: Enter Bootloader Mode (if needed)

  If the board isn't responding or you can't flash normally:

  From tech-details.md (lines 31-37):
  - Hold ▼ (Page down/D0) - the BOOT button
  - Press Reset (the button on the left side)
  - Release ▼ (Page down/D0)

  The board will restart in bootloader mode and can be re-flashed.

  Step 3: Build the Firmware

  # Build only (check for compilation errors)
  pio run

  This compiles but doesn't upload. Check for errors first!

  Step 4: Flash to the Board

  # Normal flash (SAFE - preserves your data)
  pio run -t upload

  What happens:
  - Firmware uploads to the inactive OTA partition
  - Board reboots into new firmware
  - All your data remains intact (WiFi, cards, Navagotchi state)

  Step 5: Monitor Serial Output

  # Watch what's happening
  pio device monitor

  You'll see boot messages, Navagotchi loading state, etc.

  ---
  ⚠️ Important Notes

  Current Firmware Version

  From platformio.ini line 71:
  -DCURRENT_FIRMWARE_VERSION="\"0.1.5\""

  Partition Layout (from partitions.csv)

  The board has:
  - ~1.5MB per app partition (dual OTA setup)
  - Separate NVS partition for your data
  - This is why regular flashing is safe!

  If Something Goes Wrong

  Boot loop or weird behavior?
  1. Try normal flash first: pio run -t upload
  2. If that fails, use bootloader mode (hold BOOT, press Reset, release BOOT)
  3. Last resort: Erase Flash and Upload from PlatformIO menu (⚠️ deletes everything)

  After Flashing

  1. Board reboots automatically
  2. Check serial monitor: pio device monitor
  3. Look for: "Navagotchi: Loaded state - Hunger: 50, Happiness: 50, Energy: 50"
  4. Access web UI to add the Navagotchi card
  5. Press Up/Down to navigate to it, Center button to interact!

  ---
  🎯 Quick Command Reference

  # Check for errors (doesn't touch board)
  pio run

  # Safe flash (preserves data)
  pio run -t upload

  # Watch serial output
  pio device monitor

  # All in one (build, flash, monitor)
  pio run -t upload && pio device monitor

  Your Navagotchi pet's stats will persist across flashes! The state is saved to NVS
  every time you feed/play/sleep, so even if you reflash, your pet will remember its
  stats. 🐾

  Ready to flash when you are! Let me know if you hit any issues.
  