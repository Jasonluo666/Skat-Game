import numpy as np
from keras.models import Sequential
from keras.models import load_model 
from keras.layers import Dense, Dropout
import h5py
import re

# load the existing NNW
model = [load_model('C:/Users/ljsPC/Desktop/NNW_Player/NNW_SKAT_turn0.h5')
        ,load_model('C:/Users/ljsPC/Desktop/NNW_Player/NNW_SKAT_turn1.h5')
        ,load_model('C:/Users/ljsPC/Desktop/NNW_Player/NNW_SKAT_turn2.h5')
        ,load_model('C:/Users/ljsPC/Desktop/NNW_Player/NNW_SKAT_turn3.h5')
        ,load_model('C:/Users/ljsPC/Desktop/NNW_Player/NNW_SKAT_turn4.h5')
        ,load_model('C:/Users/ljsPC/Desktop/NNW_Player/NNW_SKAT_turn5.h5')
        ,load_model('C:/Users/ljsPC/Desktop/NNW_Player/NNW_SKAT_turn6.h5')
        ,load_model('C:/Users/ljsPC/Desktop/NNW_Player/NNW_SKAT_turn7.h5')
        ,load_model('C:/Users/ljsPC/Desktop/NNW_Player/NNW_SKAT_turn8.h5')
        ,load_model('C:/Users/ljsPC/Desktop/NNW_Player/NNW_SKAT_turn9.h5')]

def nnw_player(current_state = "NULL", turn = 0):
    # print('input: ', current_state)    # test input

    data_split = re.split(' ',current_state)
    data_split = list(map(int, data_split[0:96]))     # convert to a list of int
    NNW_input = np.array(data_split).reshape(1,96)    # convert to an array

    # predict the action
    predict = model[turn].predict_on_batch(NNW_input)

    # convert to string
    predict = ' '.join(str(e) for e in predict[0])
    # print('predict: ', predict)

    return predict

#nnw_player('0 0 0 0 1 0 0 0 0 1 0 1 1 0 0 0 1 1 0 0 1 0 1 0 1 0 0 0 1 0 0 0 1 1 1 1 0 1 0 1 1 0 1 0 0 1 1 1 0 0 1 1 0 1 0 1 0 1 1 1 0 1 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0', 1)