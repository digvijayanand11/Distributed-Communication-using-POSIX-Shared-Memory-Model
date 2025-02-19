import zipfile
import os

# Define input and output paths
dict_file_path = "data/128CIF_RECDATA.dict"  # Replace with actual path
zip_output_path = "data/128CIF_RECDATA.zip"

# Step 1: Create a ZIP file and add the .dict file
with zipfile.ZipFile(zip_output_path, 'w', zipfile.ZIP_DEFLATED) as zipf:
    zipf.write(dict_file_path, os.path.basename(dict_file_path))

print(f"ZIP file created: {zip_output_path}")