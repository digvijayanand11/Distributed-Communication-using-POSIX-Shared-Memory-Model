##########################################################################
# author: Digvijay Anand
# last modified: 02-18-2025
##########################################################################

import torch
import numpy as np
import pandas as pd

device = 'cpu'
rec_data = torch.load("128CIF_RECDATA.dict", map_location=device)

num_images = 1

if "images" in rec_data and "labels" in rec_data:
    data_all = rec_data["images"].to(device) 
    labels_all = rec_data["labels"].to(device) 

    data_np = data_all.numpy()
    labels_np = labels_all.numpy()

    first_channel_data = data_np[:num_images, 0, :, :].astype(np.float32)
    first_channel_flattened = first_channel_data.reshape(num_images, -1)
    df_data = pd.DataFrame(first_channel_flattened)
    csv_file_path = f"first_channel_data_w_{num_images}_images.csv"
    df_data.to_csv(csv_file_path, index=False)

    print(f"Saved '{csv_file_path}' with shape {first_channel_flattened.shape}")

else:
    print("Error: 'images' or 'labels' not found in the dataset.")
