This is a Lexer for a very basic programming language.

Inputs a textfile of code and outputs a projection of the functions that the code contains to the console.

Within 'main.cpp' replace the string "test.txt" with the name of whatever file you want to parse.

Accepted programming language is very basic. Not all statements and operations are supported yet.




def selection_sort(items):
  end = len(items)
  start = 0
  index = 0

  pas = 1
  while(pas):
    mi = items[start]
    index = start

    for i in range(start, end):
      if mi > items[i]:
        mi = items[i]
        index = i

    print(items)

    items.pop(index)
    items.insert(start, mi)

    start += 1

    if start == end:
      pas = 0


  pass
  #TODO: write code to implement the selection sort algorithm

items = [3,1,7,0]
selection_sort(items)
print(items) #should print [0, 1, 3, 7]
