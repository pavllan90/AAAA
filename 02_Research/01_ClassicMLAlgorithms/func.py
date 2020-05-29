def transform_predict(eeg_data,est, le):
    N=10
    nums = [[0 for i in range(N)]]
    for i in range(1,int(100/N)):
        nums.append([i for j in range(N)])
        nums
    nums = np.array(nums).reshape((-1,))
    rebuild_data = []
    names_list = []
    sample_by_data = eeg_data
    sample_by_data['sub_period'] = nums
    accs = pd.Series()
    for part in eeg_data['sub_period'].unique():
        part_data = sample_by_data[sample_by_data['sub_period']==part]
        for i in range(14):
            accs['mean_'+str(i)+"_"+str(part)] = part_data[str(i)].mean(axis = 0)
            accs['min_'+str(i)+"_"+str(part)] = part_data[str(i)].min(axis = 0)
            accs['max_'+str(i)+"_"+str(part)] = part_data[str(i)].max(axis = 0)
            names_list.append('mean_'+str(i)+"_"+str(part))
            names_list.append('min_'+str(i)+"_"+str(part))
            names_list.append('max_'+str(i)+"_"+str(part))
    rebuild_data.append(accs.values)
    rebuild_data = pd.DataFrame(rebuild_data)
    rebuild_data.columns = names_list
    return le.inverse_transform(estimator.predict(rebuild_data))
