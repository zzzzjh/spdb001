""" 
Author Zjh
2019/1/5
by-Xaut
"""
import cv2
import numpy as np
import matplotlib.pyplot as plt
l=[]
dic={}
rect={}
label=0
""" 种子填充法进行填充 """
def fill(im):
    global label
    row,col=im.shape[:2]
    for i in range(row):
        for j in range(col):
            if im[i,j]==255:
                l.append([i,j])
                seed=(i,j)
                label=label+1
                row_min=i
                row_max=i
                col_min=j
                col_max=j
                dic[label]=np.random.randint(0,255,(1,3))#产生随机的颜色，对不同的连通区域进行标记
                im[i,j]=label #标记
                while(len(l)!=0):
                    """遍历四个邻域"""
                    if im[seed[0],seed[1]+1]==255:
                        im[seed[0],seed[1]+1]=label
                        l.append([seed[0],seed[1]+1])
                        if col_max<seed[1]+1:
                            col_max=seed[1]+1
                    if im[seed[0]+1,seed[1]]==255:
                        im[seed[0]+1,seed[1]]=label
                        l.append([seed[0]+1,seed[1]])
                        if row_max<seed[0]+1:
                            row_max=seed[0]+1
                    if im[seed[0],seed[1]-1]==255:
                        im[seed[0],seed[1]-1]=label
                        l.append([seed[0],seed[1]-1])
                        if col_min>seed[1]-1:
                            col_min=seed[1]-1
                    if im[seed[0]-1,seed[1]]==255:
                        im[seed[0]-1,seed[1]]=label
                        l.append([seed[0]-1,seed[1]])
                        if row_min>seed[0]-1:
                            row_min=seed[0]-1
                    seed=l[-1]
                    l.pop()#相当于堆栈出栈的过程
                rect[label]=[col_min,row_min,col_max,row_max]
    color(im)
""" 绘制矩形框 """
def color(img):
    row,col=img.shape
    out=np.zeros((row,col,3),np.uint8)
    for i in range(row):
        for j in range(col):
            d=img[i,j]
            if d!=0:
                out[i,j]=dic[d]
    for x in rect.keys():
        cv2.rectangle(out,(rect[x][0],rect[x][1]),(rect[x][2],rect[x][3]),(0,255,0),2)
    cv2.imshow("Annie",out)
    cv2.waitKey(0)
img=cv2.imread("c:\\users\\xaut\\desktop\\test.jpg",0)
im=cv2.threshold(img,50,255,cv2.THRESH_BINARY)[1]
fill(im)