import gensim, nltk, os
import plotly.express as px
import pandas as pd
import numpy as np
import pypdf
import re
import logging
#import spacy

logging.basicConfig(format='%(asctime)s : %(levelname)s : %(message)s', level=logging.INFO)
#downloaders
#nltk.download('wordnet')
#main texts:
#Analects, Dao De Jing, Mengzi, Zhuangzi
#pdf text
titles = []
texts = []
cleaned_texts = []

#cn texts
titlescn = []
textscn = []

#second en translation texts
titles2 = []
texts2 = []

#second en translation texts split by chapter
titles3 = []
texts3 = []

#lists to store everything after whitelist parse
most_important_words_texts1 = []
most_important_words_texts2 = []
most_important_words_texts3 = []

biglist_titles = []
biglist_texts = []
#regex to remove formatting strings after parsing data
#aggressive parsing stuff
rexpr3 = re.compile(r"[a-zA-Z']+")
rexpr4 = re.compile(r"[^a-zA-Z0-9：¶#:，。；！「」<>]+")

#remove common, nonimportant words from the list, just in case
blacklist = [" the ", " is ", " are ", " of ", " i "]

#to parse the texts and grab very important phrases for a separate topic modelling
#either as pinyin or as their rough English translations. The shorter the better
#format is PINYIN, ENGLISH TRANSLATION(s), alternating
whitelist = [
             "dao", "way", "de", "power", "virtue", "wuwei", "wu wei", "yin",
             "yang", "ziran", "naturalness", "spontaneity", "pu",
             "simplicity", "wu", "emptiness", "sage", "ren", "benevolence"
             "xin", "heart", "junzi", "gentleman", "noble", "li", "duty", "tian", "heaven",
             "zhong", "faithfulness", "loyalty", "jun", "ruler", "king", "xiao", "filial",
             "xing", "nature", "disposition", "qi", "life", "youyou", "wandering", "dream", "reality",
             "zhi xing", "zhixing", "conduct", "zhongyong", "doctrine of the mean", "harmony", "balance", "moderation"
             ]
#basic testing flag. If set to true, just runs the algorithm over the chapters of The Analects
#otherwise, run over all texts
test = False
if(test):
    for root, dirs, files in os.walk('analects_split'):
        #print(files)
        files = [file for file in files if file[0] != '.']
        for f in files:
            print(f)
            #text = ''
            with open(os.path.join(root, f), 'r', encoding='utf8') as rf:
                text = rf.read()
                texts.append(' '.join(rexpr3.findall(text)))
                titles.append(f[:-4])
else:
    #run over all files in "texts", which is made up of only full texts
    for root, dirs, files in os.walk('texts'):
        files = [file for file in files if file[0] != '.']
        for f in files:
            print(f)
            if f[-4:] == ".pdf":
                reader = pypdf.PdfReader(os.path.join(root, f))
                text = ""
                #print(reader.pages[0])
                for page in reader.pages:
                    text += page.extract_text() + " "
                texts.append(text[:-1])
                #print(text)
                titles.append(f[:-4])
                
                parser = ''
                for ele in whitelist:
                    #use titles3 as titles for the list
                    parser += ' '.join(re.findall(ele, text))
                most_important_words_texts1.append(parser)

            elif f[-4:] == ".txt":
                if f[-6:-4] == "cn":
                    with open(os.path.join(root, f), 'r', encoding='utf8') as rf:
                        text = rf.read()
                        textscn.append(' '.join((''.join(rexpr4.findall(text))).split()))
                        titlescn.append(f[:-4])
                elif f[-6:-4] == "en":
                    text = ''
                    with open(os.path.join(root, f), 'r', encoding='utf8') as rf:
                        text = rf.read()
                        for ele in blacklist:
                            text = text.replace(ele, " ")
                        texts2.append(' '.join(rexpr3.findall(text)))
                        titles2.append(f[:-4])

                        parser = ''
                        for ele in whitelist:
                            #use titles3 as titles for the list
                            parser += ' '.join(re.findall(ele, text))
                        most_important_words_texts2.append(parser)
    
    #get the chapter-split english corpus, split by work
    for root,dirs,files in os.walk('texts_en_split'):
        files = [file for file in files if file[0] != '.']
        for f in files:
            text = ''
            with open(os.path.join(root, f), 'r', encoding='utf8') as rf:
                text = rf.read()
                for ele in blacklist:
                    text = text.replace(ele, " ")
                texts3.append(' '.join(rexpr3.findall(text)))
                
                #save title as the path
                titles3.append((os.path.join(root, f)[:-4]).replace("/","_"))

                parser = ''
                for ele in whitelist:
                    #use titles3 as titles for the list
                    parser += ' '.join(re.findall(ele, text))
                most_important_words_texts3.append(parser)

#print(most_important_words_texts1, most_important_words_texts2, most_important_words_texts3)

#exit()

        
        
for text in texts:
    #parse the texts and remove all the pdf formatting strings from pdf texts
    res = rexpr3.findall(text)
    cleaned_texts.append(' '.join(res))
    #print(len(text), len(''.join(res)))
    #print(res)
#print(cleaned_texts)
lemmatizer = nltk.WordNetLemmatizer()

#compile all the individual lists for easier iteration through them
biglist_titles.append(titles)
biglist_titles.append(titles2)
biglist_titles.append(titlescn)
biglist_titles.append(titles3)
biglist_titles.append(titles)
biglist_titles.append(titles2)
biglist_titles.append(titles3)

biglist_texts.append(cleaned_texts)
biglist_texts.append(texts2)
biglist_texts.append(textscn)
biglist_texts.append(texts3)
biglist_texts.append(most_important_words_texts1)
biglist_texts.append(most_important_words_texts2)
biglist_texts.append(most_important_words_texts3)

i = 0

#for exporting images
#import kaleido

def plot_topic(topic_num, index, topn=20):
        topic_data = {"words":[], "weights":[]}

        for word, weight in lda.show_topic(topic_num, topn=topn):
            topic_data["words"].append(word)
            topic_data["weights"].append(weight)

        df = pd.DataFrame(topic_data)

        fig = px.bar(df, x="words", y="weights")
        fig.show()
        #fig.write_image(f"topics{index}.png")
        return df

#iterate through all sets of corpuses
#i.e. Every PDF version of the four texts, every Chinese version of the four texts, etc.
for t in biglist_texts:
    if test and i != 0:
        break
    refined_texts = []
    for text in t:
        tokenized_text = nltk.word_tokenize(text)
        refined = [lemmatizer.lemmatize(word).lower() for word in tokenized_text if word.isalnum()]
        refined_texts.append(refined)

    #print(refined_texts)
    corpus_dictionary = gensim.corpora.Dictionary(refined_texts)
    # no below: term must appear in at least 5% (length of the list of refined texts * 0.05) of documents
    # no above: term cannot appear in more than 75 percent of documents
    corpus_dictionary.filter_extremes(no_below=int(len(refined_texts) * .25) + 1, no_above=.75)
    print(corpus_dictionary)

    # prep corpus for gensim
    processed_corpus = [corpus_dictionary.doc2bow(text) for text in refined_texts]

    # gensim.corpora.Mmcorpus.serialize('mycorpus.mm', processed_corpus)
    # gensim.corpora.Mmcorpus('mycorpus.mm')

    lda = gensim.models.ldamodel.LdaModel(
        #processed_corpus, num_topics=len(refined_texts), id2word=corpus_dictionary, iterations=500, passes=50
        processed_corpus, num_topics=6, id2word=corpus_dictionary, iterations=500, passes=50
    )

    topics = lda.show_topics()
    for topic in topics:
        print(topic)

    #topic_df = plot_topic(len(refined_texts)-1, i, topn=15)
    topic_df = plot_topic(5, i, topn=15)


    doc_lda = lda.get_document_topics(processed_corpus[0], minimum_probability=0.0)
    doc_data = {"topic_num":[d[0] for d in doc_lda], "topic_weight":[d[1] for d in doc_lda]}
    print(doc_lda)
    doc_df = pd.DataFrame(doc_data)
    fig = px.bar(doc_df, x="topic_num", y="topic_weight")
    #fig.labels()
    fig.show()
    #save figure as image
    #fig.write_image(f"documents{i}.png")

    # look at topic prevelance across full corpus
    # get all topics
    all_doc_topics = [lda.get_document_topics(processed_corpus[i],minimum_probability=0.0) for i in range(len(processed_corpus))]
    #print(np.array(all_doc_topics), "\n", np.sum(np.array(all_doc_topics), axis=1))
    old_corpus_lda = np.sum(np.array(all_doc_topics), axis=1)
    corpus_lda = np.sum(old_corpus_lda, axis=1)
    corpus_lda = corpus_lda - corpus_lda.mean()
    #print("**************\n",corpus_lda,"\n**************\n")
    corpus_data = {"topic_num":[i for i in range(len(processed_corpus))], "topic_weight":corpus_lda}
    corpus_df = pd.DataFrame(corpus_data)
    fig = px.bar(corpus_df, x="topic_num", y="topic_weight")
    fig.show()
    #save figure as image
    #fig.write_image(f"corpus{i}.png")

    i += 1
    #save to tsv files, not necessary
    """ doc_df.to_csv(f"document_lda{i}_test_{test}.tsv", sep='\t')
    topic_df.to_csv(f"topics{i}_test_{test}.tsv", sep='\t')
    corpus_df.to_csv(f"corpus{i}_test_{test}.tsv", sep='\t')
    old_corpus_df = pd.DataFrame(old_corpus_lda)
    old_corpus_df.to_csv(f"corpus{i}_old_test_{test}.tsv", sep='\t') """
    #print(i)