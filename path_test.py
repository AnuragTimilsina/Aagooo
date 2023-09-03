import os
from pathlib import Path

BASE_DIR = Path(__file__).resolve().parent
print("BaseDIR: ", BASE_DIR)

Image_path_join = os.path.join(BASE_DIR, "saved_infer_image")
print("Image path: ", Image_path_join)

