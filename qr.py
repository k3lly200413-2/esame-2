import qrcode
import os

url = input("ENTER URL: ").strip()

user_home = os.path.expanduser("~")
path = os.path.join(user_home, "Desktop", "qr_code_made.png")

# path = "C:\\Users\\Kelly Applebee\\Desktop\\qrcodeMade.png"

qr = qrcode.QRCode()
qr.add_data(url)

img = qr.make_image()
img.save(path)

print(f"done saved to {path}")

input("Press Enter to Close...")
