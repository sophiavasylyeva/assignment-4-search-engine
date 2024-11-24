# Assignment 4: Answers

**Complete this document, commit your changes to Github and submit the repository URL to Canvas.** Keep your answers short and precise.

Names of all team members: Sophia Vasylyeva, Jennifer Galindo, Joseph Skiba

Used free extension: [x] 24 hrs or [ ] 48 hrs

[ ] Early submission (48 hrs)

[x] Bonus work. Describe: ...

Place [x] for what applies.


## Answers

### Project Management

Complete the tasks and planned completion columns **BEFORE** You start with 
coding!


Weekly milestones/tasks. Set out a plan at the beginning and then record when you got it done.

| Task              | Planned completion | Actual completion | Time spent in hrs by Jennifer | Time spent in hrs by Sophia |  Time spent in hrs by Joseph |
| -----------       | :-----------------:| :---------------: | :---------------------------: | :-------------------------: |  :-------------------------: |
|  AVL_Tree         |       Nov 4        |      Nov 11       |               0               |              0              |              10              |
|  Hashing          |       Nov 4        |      Nov 11       |               10              |              0              |              0               |
|  QueryProcessor   |       Nov 11       |      Nov 20       |               15              |              1              |              0               |
|  Doc              |       Nov 11       |      Nov 16       |               0               |              1              |              0               |
|  DocParser        |       Nov 18       |      Nov 29       |               0               |              15             |              0               |
|  IndexHandler     |       Nov 18       |      Nov 25       |               2               |              10             |              1               |  
|  Persistence      |       Nov 25       |      Dec 1        |               0               |              0              |              5               |
|  UserInterface    |       Nov 25       |      Dec 1        |               0               |              0              |              7               |
|  Testing/Debug    |       Nov 29       |      Dec 1        |               1               |              1              |              3               |
|  Answers/UML      |       Nov 29       |      Dec 1        |               3               |              3              |              2               |  
 



### High-level Design of Solution

- Add a UML class diagram, etc.
https://docs.google.com/document/d/1b4gdLtm3oQlbAPjj6ntwAI5PmmyeGXKXjSKIzXMcrEo/edit?usp=sharing

- High-level pseudo code for each component.
Doc Parser  
    given folder path, get all file names in folder 
    parse all JSONs in file names 
        tokenize - lowercase 
        stemming 
        store processed data in index 
        remove stop words 
Doc 
    Initializing parameters of the documents
        Uuid
        Title
        Publication
        Date
        Total Word Count
        Raw Text
        Relevancy 
Index Handler
    AVL trees - ordered    
        binary search tree 
        AVLTree <string <-- word, AVLTree<string<--doc UUID, size_t>> - (for every word: store map of docs that word is in, and for each of docs we also store the count of that word)  
        Each for people, organization, words
    Getters for words, people, organization, and # of docs
    Creates and adds words, people, organization to vector of data 
        Data stores all documents (internal storage)
    Send requested data to a query engine 
    Persistence
        handles persistence files 
            Prints # of docs 
            Iterate through docs 
            Print doc data 
            Print #of unique words 
            Iterate through nodes 
            Print words, docs and frequencies  
            map for words: key = word, value = document appears in 
        Loads words, people, organization
    catch2 testing 
Query Processor   
    gets string query from user 
    deciphers what query means (person, org, etc,) 
    requests data from index handler 
    Gets list of docs from query  
    Sort docs by relevancy 
    Calculate TF IDF 
    Return docs by order of relevancy  
    Gets lists of docs from query 
    sort docs by relevancy (TF_IDK) 
    returns docs in order of relevancy 
    tf-idf 
        N word, doc = count of words in doc 
        N doc = total word count in doc 
        N word = # of docs that include words 
        N = total # of docs //maybe vector that keeps track of all the documents you have seen  
        tf = n word, doc/ n doc 
        idf = log 2(N / N word) 
        tf-idf = (tf)(idf) 
Hashing
    Hash Table are Tables with a set capacity that store keys to help us find stored elements 
    Hash table do not store keys in a particular order
    Hash Table distributes keys well so we don't have a table that is mostly empty 
    Runtime O(1)
    Find
        You are able to find a value quickly and return it into a map 
    Size
        Returning size
    Insert
        You are able to insert a value into the Hash table .
    Rehash
        Doubling capacity, creating new Hash Table, and inserting the values into new Table
    Clear
        Deleting everything in Hash Table and resetting the table with a capacity of a hundred
User Interface 
    When opened by the command-line arguments, the UI presents a menu
    Depending on the selected option, the menu will allow the user to either:
        Create a new index (the user submits a directory)
        Save the new index
        Load a saved index found in 4 .txt files
        Enter a query (the user submits a directory)
            When the query is processed, the queried documents are presented by relevance.
            The user has the option to view a specific document’s text.
        Show common statistics
AVL Tree
    AVL Trees are a container type that use nodes to store data and allows for quick searching.
    AVL Trees are stored in a way that the parent node will be greater than its left child node and less than its right child node.
    IsEmpty
        Checks if AVL Tree is empty
    makeEmpty
        Removes all nodes from the AVL Tree
    treeSize
        Returns the number of nodes in the AVL Tree
    Insert
        Inserts a node with a key and its respective value (map of int and int)
    Remove
        Removes a specified node from the AVL Tree
    Find
        Finds a node with a specified key and returns its respective map
    SaveTree
        Creates a hard copy of each node from the AVL Tree into a .txt file


### Used Data Structures
Explanation of what data structures are used where. For each, state
    - time and space complexity (Big-Oh),
    - why it is the appropriate data structure and what other data structure could have been used.

We used a map in the AVL tree and hashing. A map has a time complexity of O(log N).
We used a hash table in hashing.h to speed up the query processing. A hash table has a time complexity of O(1).
The hash table is an alternative to the AVL tree. Both serve the same purpose, but the hash table is a faster implementation. 
We used a map, but a set could have been an alternative structure. A map allowed us to be able to calculate the frequency of the word in the document, because of the key and value structure. 

### User Documentation
- What are the features of your software?
The software is opened through three different command-line argument “modes”
    index <user directory>
        Opens the directory, creates an index, saves the index via .txt files.
    query <user query>
        Submits the user’s query to the processor, outputs the most relevant document titles.
    UI
        Allows the user to manually enter in their own index or query.

- Describe how to use the software.
To use the software, the user must first decide what “mode” they wish to enter.
    If the mode is “index”, the software simply creates and saves an index without any user inputs.
    If the mode is “query”, the software searches the given query and allows the user to view any relevant document.
    If the mode is “ui”, the software allows the user to manually create, save, load, and search through a given directory.

- Some example queries with the results.
./supersearch query firm
     [Here are the most relevant documents!]     

1.)   German firms doing business in UK gloomy about Brexit - survey
         reuters.com
        2018-02-27T20:09:00.000+02:00

./supersearch query fiscal PERSON:eaton
     [Here are the most relevant documents!]     

1.)   Eaton Vance Closed-End Funds Release Estimated Sources Of Distributions
         cnbc.com
        2018-02-28T18:54:00.000+02:00

./supersearch query now ORG:eu
    [Here are the most relevant documents!]     

1.)   German firms doing business in UK gloomy about Brexit - survey
         reuters.com
        2018-02-27T20:09:00.000+02:00

### Performance
- Provide statistics (word counts, timing, etc) for indexing all documents (or as many as you can in a reasonable amount of time).

For a small set of 8,000 documents, it took our program 18 seconds to parse, with a total of over 10,0000 different words.
For a large set of 100,000 documents, it took our program around 20 minutes to fully parse, with a total of over 15,000 words.

### Bonus Work
- Did you attempt any bonuses? If so, please give a brief description of what you did.

Indexing and query speed on a smaller dataset
    Using the sample_data folder included in the project, we got the following index and query speed using ./supersearch query firm…
Hashing
    Attempted HashTable and HashTable Testing to increase speed of query processor but did not implement in index handler.
Relevancy ranking
    Calculating TF IDF and ordering it with a merge sort.
