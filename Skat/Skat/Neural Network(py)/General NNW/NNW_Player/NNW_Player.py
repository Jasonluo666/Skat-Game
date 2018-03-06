import numpy as np
from keras.models import Sequential
from keras.models import load_model 
from keras.layers import Dense, Dropout
import h5py
import re

# load the existing NNW
model = load_model('C:/Users/ljsPC/Desktop/NNW_Player/NNW_SKAT.h5')

def nnw_player(current_state = "NULL"):
    # print('input: ', current_state)    # test input

    data_split = re.split(' ',current_state)
    data_split = list(map(int, data_split[0:96]))     # convert to a list of int
    NNW_input = np.array(data_split).reshape(1,96)    # convert to an array

    # predict the action
    predict = model.predict_on_batch(NNW_input)

    # convert to string
    predict = ' '.join(str(e) for e in predict[0])
    # print('predict: ', predict)

    return predict

# nnw_player('0 0 0 0 1 0 0 0 0 1 0 1 1 0 0 0 1 1 0 0 1 0 1 0 1 0 0 0 1 0 0 0 0 0 0 0 1 0 1 0 0 1 0 1 1 0 0 0 1 1 0 0 1 0 1 0 1 0 0 0 1 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ')