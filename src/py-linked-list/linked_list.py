#!/usr/bin/python
#-*- coding: utf-8 -*-
'''
	abs. : реализация абстрактного списка
 
'''
import os
import sys

class Node:
    def __init__(self,value):
        self.data = value
        self.next = 0
 
class List:
    def __init__(self):
        self.firstNode = Node(0)  # создаем узел уже в конструкторе
 
    # рекурсивный вызов
    def __ShowNodeData(self,aNode):
        if aNode.next != 0:
           print aNode.data
           self.__ShowNodeData(aNode.next)
 
    # показать все
    def Dump(self):
        self.__ShowNodeData(self.firstNode)
 
    # вставка
    def InsertAfter(self,aNode,aNewNode):
        aNewNode.next = aNode.next
        aNode.next = aNewNode
 
    def InsertBeginning(self,aNewNode):
        aNewNode.next = self.firstNode
        self.firstNode = aNewNode    
 
# объявляем узлы
nodeA = Node("A")
nodeB = Node("B")
nodeC = Node("C")
nodeD = Node("D")
 
# создаем список и заполняем его
aList = List()
 
aList.InsertBeginning(nodeB)
aList.InsertAfter(nodeB,nodeD)
aList.InsertAfter(nodeD,nodeC)
aList.InsertAfter(nodeC,nodeA)
 
aList.Dump()
