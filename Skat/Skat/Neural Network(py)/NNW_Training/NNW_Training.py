def NNW_Training(training_file = '', layer = 0):
    import numpy as np
    from keras.models import Sequential
    from keras.models import load_model 
    from keras.layers import Dense, Dropout
    import h5py

    # load the existing NNW
    model = load_model('NNW_SKAT_turn' + str(layer) + '.h5')

    '''
    # create a new NNW
    model = Sequential()
    model.add(Dense(output_dim = 32, input_dim = 96, activation='sigmoid'))

    model.compile(loss='mse', optimizer='sgd')
    '''

    import sys
    import re

    # load training data
    training_data = open(training_file, 'r')
    line = training_data.readline()

    while line:
        # print (line)
        line_split = re.split(' ',line)
        line_split = list(map(int, line_split[0:128]))     # convert to a list of int
        print (line_split)
        x_train = np.array(line_split[0:96]).reshape(1,96)
        y_train = np.array(line_split[96:128]).reshape(1,32)
        # print (x_train)
        model.train_on_batch(x_train, y_train)
        line = training_data.readline()

    training_data.close()

    '''
    # epoch: times that train data is used
    # batch_size: batches (in total) used for training
    model.fit(x_train, y_train, epochs = 10, batch_size = 50)
    accuracy = model.evaluate(x_test, y_test, batch_size = 10)
    print('accuracy: ', accuracy);
    '''

    print('y_train: ', y_train);
    predict = model.predict_on_batch(x_train);
    print('predict: ', predict);

    '''
    sample = np.random.random((1, 64))
    print('sample: ', sample);
    # predict the action for sample
    predict = model.predict_on_batch(sample);
    print('predict: ', predict);
    '''

    # save the NNW
    model.save('NNW_SKAT_turn' + str(layer) + '.h5')

# call the training function
NNW_Training('./training_data0.txt', 0)
NNW_Training('./training_data1.txt', 1)
NNW_Training('./training_data2.txt', 2)
NNW_Training('./training_data3.txt', 3)
NNW_Training('./training_data4.txt', 4)
NNW_Training('./training_data5.txt', 5)
NNW_Training('./training_data6.txt', 6)
NNW_Training('./training_data7.txt', 7)
NNW_Training('./training_data8.txt', 8)
NNW_Training('./training_data9.txt', 9)
