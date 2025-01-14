#%%
import requests
import numpy as np
import time

from cortex2 import EmotivCortex2Client
import time
import keyboard
import numpy as np
import pandas as pd
from sklearn.ensemble import RandomForestClassifier
import pickle
from test_pipeline_3 import *
#from test_pipeline import *

predictor = TestPipelineEEG(image_size = 28, frame_duration = 0.78, overlap = 0.0, model_path = 'model_1605.h5')
url = "wss://localhost:6868"
client = EmotivCortex2Client(url,
                             client_id='',
                             client_secret='',
                             check_response=True,
                             authenticate=True,
                             debug=False,
                             data_deque_size = 200)



# Test API connection by using the request access method
client.request_access()

# Explicit call to Authenticate (approve and get Cortex Token)
client.authenticate()

# Connect to headset, connect to the first one found, and start a session for it
client.query_headsets()
client.connect_headset(0)
client.create_activated_session(0)
client.subscribe(streams=['eeg'])
while True:
    try:
        eeg = []
        if len(list(client.data_streams.values())[0]['eeg']) == 200:
            data = list(client.data_streams.values())[0]['eeg']
            for i in data:
                eeg.append(i['data'][2:16])
        
        # ML
        eeg_data = pd.DataFrame(eeg)
        overlap = 0.5
        #overlap = 0
        lst = []
        i = 0
        while (i+100) <= eeg_data.shape[0]:
            lst.append(predictor.evaluate(eeg_data.iloc[i:i+100], threshold = 0.5))
            i = i + 100 - int(100*overlap)
        
        smooth_list = lst
        label = max(set(smooth_list), key=smooth_list.count)
        if label == 0:
            print('Ладонь')
        elif label == 2:
            print('Ножницы')
        elif label == 1:
            print('Кулак')
        time.sleep(1.5)
    except:
        print('--')
        pass

# %%
