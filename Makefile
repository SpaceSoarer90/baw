build:
	arduino-cli compile
upload: build
	arduino-cli upload
