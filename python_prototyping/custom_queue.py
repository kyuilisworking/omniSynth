class Queue_Node:

    def __init__(self, val):
        self.previous = None
        self.next = None
        self.val = val

    def set_next(self, next_node):
        self.next = next_node

    def set_previous(self, previous_node):
        self.previous = previous_node

    def set_val(self, val):
        self.val = val

    def get_next(self):
        return self.next

    def get_previous(self):
        return self.previous

    def get_val(self):
        return self.val

class Custom_Queue:

    '''
    Specified for use in detect_whistle.py
    Works with numerical values
    '''

    def __init__(self, max_size = 10):
        '''
        Constructor
        Arguments:
            max_size is the total number of elements in the Queue
        '''
        self.first_node = None
        self.last_node = None
        self.size = 0
        self.max_size = max_size

    def is_empty(self):
        '''
        Returns true if no elements
        '''
        if self.size == 0:
            return True
        else:
            return False

    def update(self, new_val):
        '''
        Adds new_val to the Queue
        Deletes previous values if necessary to keep max_size
        '''
        new_node = Queue_Node(new_val)
        if self.size == 0:
            #Empty Queue
            self.first_node = new_node
            self.last_node = self.first_node
            self.size += 1
        elif self.size == self.max_size:
            #Full Queue
            self.first_node.set_previous(new_node)
            new_node.set_next(self.first_node)
            self.first_node = new_node
            self.last_node = self.last_node.get_previous()
            self.last_node.set_next(None)
        else:
            #Partially Full Queue
            self.first_node.set_previous(new_node)
            new_node.set_next(self.first_node)
            self.first_node = new_node
            self.size += 1

    def pop(self):
        '''
        Pops the last value of the Queue
        '''

        if self.size == 0:
            return None
        elif self.size == 1:
            node = self.first_node
            self.last_node = None
            self.first_node = None
            self.size -= 1
            return node
        else:
            node = self.last_node
            self.last_node = self.last_node.get_previous()
            self.last_node.set_next(None)
            self.size -= 1
            return node

    def get_array(self):
        current_node = self.first_node
        ret_array = []
        while current_node != None:
            ret_array.append(current_node.get_val())
            current_node = current_node.get_next()
        return ret_array






