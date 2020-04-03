from lib.embedding import word2vec
path_model="./data/GoogleNews-vectors-negative300.bin" # input model 
path_doc= "./bbc_brazil_meat.txt" # input text 
obj=word2vec.Word2Vec()
print(obj.myadd(1,2))
print(obj.doc2vector(path_doc,path_model))
