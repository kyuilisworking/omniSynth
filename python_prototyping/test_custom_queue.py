from custom_queue import Custom_Queue

q = Custom_Queue(5)
q.update(3)
q.update(4)
q.update(5)
q.update(6)
q.update(7)
q.update(8)
q.update(9)
q.pop()
q.pop()
q.pop()
q.pop()
q.pop()
q.pop()
q.pop()

print q.get_array()