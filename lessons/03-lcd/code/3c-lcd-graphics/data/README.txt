Place a JPEG image here named demo.jpg.

Expected path in this project:
  data/demo.jpg

Then run the PlatformIO task:
  Upload Filesystem Image

The graphics demo will load it from SPIFFS using:
  drawJpgFile(SPIFFS, "/demo.jpg", x, y)
