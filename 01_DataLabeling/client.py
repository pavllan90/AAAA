from cortex2 import EmotivCortex2Client
import time
import keyboard
import numpy as np
import pandas as pd
from GestureDetector import *

data = []
url = "wss://localhost:6868"
count_number = 0
detector = GestureDetector()
    # Start client with authentication
client = EmotivCortex2Client(url,
                            client_id='X',
                            client_secret="X",
                            check_response=True,
                            authenticate=True,
                            debug=False)
client.request_access()

client.query_headsets()
client.connect_headset(0)

client.create_activated_session(0)
#client.create_session(0)
#client.subscribe(streams=["mot"])
client.subscribe(streams=["eeg"])
#print(client.receive_data()['eeg'][2:16])

client.pause_subscriber()

while True:
    command_start = input()

    if command_start == 'x':
        break

    try:
        data_curr = []
        gesture_1 = detector.get_gesture()
        start_time = time.time()

        client.resume_subscriber()

        for i in range(100):
            data_curr.append([count_number] + client.receive_data()['eeg'][2:16])

        client.pause_subscriber()

        gesture_2 = detector.get_gesture()
        ## join gesture_1 and gesture_2
        print("---------------------------------------------- %s seconds -------------------------------------------" % (time.time() - start_time))

        gesture = str(gesture_1[0])+'_'+str(gesture_2[0])
        if gesture == '0_1':
            gesture = 'разжать руку'
        elif gesture == '1_0':
            gesture = 'сжать руку'
        print("---------------------------------------------- gesture is %s -------------------------------------------" % (gesture))

        print('---------------------------------------------- Is it correct  ? (q - yes, e - no) -----------------------------------------')
        command_check = input()
        if command_check == 'q':
            for i in data_curr:
                i.append(gesture)
            data = data + data_curr
            print('---------------------------------------------- Shape - %s -------------------------------------------' % (len(data)/100))
            count_number = count_number + 1
    except:
        client.pause_subscriber()
        print('--------------------------------------------- Something wrong, no data -----------------------------------------')

#np_data = np.array(data)
df = pd.DataFrame(data)

print('------------------------- DATA SHAPE --------------------- ')
print(df.shape)

print('------------------------------------------DATA SAVED -----------------------------------------------------')
#p.savetxt("data_train.csv", np_data, delimiter=",")
df.to_csv('data_train.csv', index = False)

client.resume_subscriber()
client.stop_subscriber()
