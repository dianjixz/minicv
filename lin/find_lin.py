import math
def liner_fitting(data_x,data_y):		#最小二乘法
      size = len(data_x)
      i=0
      sum_xy=0
      sum_y=0
      sum_x=0
      sum_sqare_x=0
      average_x=0
      average_y=0
      while i<size:
          sum_xy+=data_x[i]*data_y[i]
          sum_y+=data_y[i]
          sum_x+=data_x[i]
          sum_sqare_x+=data_x[i]*data_x[i]
          i+=1
      average_x=sum_x/size
      average_y=sum_y/size
      return_k=(size*sum_xy-sum_x*sum_y)/(size*sum_sqare_x-sum_x*sum_x)
      return_b=average_y-average_x*return_k
      return [return_k,return_b]


line_list = []		#数据集列表
lines = []			#线列表

lines = [[{"x":0,"y":0,"w":0,"h":0}]]		#初始化一条线
for src in line_list:						#遍历线
	for sr in src:							#遍历线上的点
		point = False						#判断这条线是否符合
		for me in lines:					#遍历线列表，判断线是否符合重合要求
			x1 = sr["x"]
			x2 = sr["x"] + sr["w"]
			x3 = me[-1]["x"]
			x4 = me[-1]["x"] + me[-1]["w"]
			w1 = sr["w"]
			w2 = me[-1]["w"]
			xmin = min([x1,x2,x3,x4])
			xmax = max([x1,x2,x3,x4])
			if (xmax + xmin) < (w1 + w2):
				me.append(sr)
				point = True
		if point == False:					#找不到符合要求的线										
			lines.append([sr])				#加入线列表
def calculate_distance(Point1,Point2):		#计算两点之间的距离
	d_x = Point1[0] - Point2[0]
	d_y = Point1[1] - Point2[1]
	#计算两点之间的距离
	distance = math.sqrt(d_x**2 + d_y**2)
	return distance

point_line = []				#最小二乘法的线
for liness in lines:	#遍历每一条线
	point_x = []
	point_y = []
	if len(liness) < 1 :
		continue
	for ss in liness:
		point_x.append(ss["cx"])
		point_y.append(ss["cy"])
	point_line.append((liner_fitting(point_x,point_y),calculate_distance((liness[0]["cx"],liness[0]["cy"]),(liness[-1]["cx"],liness[-1]["cy"]))))#计算出每一条线的最小拟合线

max_l = 0
max_p = []
for i in point_line:			#找出最长的那条线
	if i[1] > max_l:
		max_l = i[1]
		max_p = i[0]

return max_p,max_l				#返回这条线的最小拟合值，和长度

	

































# ##最小二乘法
# import numpy as np   ##科学计算库 
# import scipy as sp   ##在numpy基础上实现的部分算法库
# import matplotlib.pyplot as plt  ##绘图库
# from scipy.optimize import leastsq  ##引入最小二乘法算法

# '''
#      设置样本数据，真实数据需要在这里处理
# '''
# ##样本数据(Xi,Yi)，需要转换成数组(列表)形式
# Xi=np.array([6.19,2.51,7.29,7.01,5.7,2.66,3.98,2.5,9.1,4.2])
# Yi=np.array([5.25,2.83,6.41,6.71,5.1,4.23,5.05,1.98,10.5,6.3])

# '''
#     设定拟合函数和偏差函数
#     函数的形状确定过程：
#     1.先画样本图像
#     2.根据样本图像大致形状确定函数形式(直线、抛物线、正弦余弦等)
# '''

# ##需要拟合的函数func :指定函数的形状
# def func(p,x):
#     k,b=p
#     return k*x+b

# ##偏差函数：x,y都是列表:这里的x,y更上面的Xi,Yi中是一一对应的
# def error(p,x,y):
#     return func(p,x)-y

# '''
#     主要部分：附带部分说明
#     1.leastsq函数的返回值tuple，第一个元素是求解结果，第二个是求解的代价值(个人理解)
#     2.官网的原话（第二个值）：Value of the cost function at the solution
#     3.实例：Para=>(array([ 0.61349535,  1.79409255]), 3)
#     4.返回值元组中第一个值的数量跟需要求解的参数的数量一致
# '''

# #k,b的初始值，可以任意设定,经过几次试验，发现p0的值会影响cost的值：Para[1]
# p0=[1,20]

# #把error函数中除了p0以外的参数打包到args中(使用要求)
# Para=leastsq(error,p0,args=(Xi,Yi))

# #读取结果
# k,b=Para[0]
# print("k=",k,"b=",b)
# print("cost："+str(Para[1]))
# print("求解的拟合直线为:")
# print("y="+str(round(k,2))+"x+"+str(round(b,2)))

# '''
#    绘图，看拟合效果.
#    matplotlib默认不支持中文，label设置中文的话需要另行设置
#    如果报错，改成英文就可以
# '''

# #画样本点
# plt.figure(figsize=(8,6)) ##指定图像比例： 8：6
# plt.scatter(Xi,Yi,color="green",label="样本数据",linewidth=2) 

# #画拟合直线
# x=np.linspace(0,12,100) ##在0-15直接画100个连续点
# y=k*x+b ##函数式
# plt.plot(x,y,color="red",label="拟合直线",linewidth=2) 
# plt.legend(loc='lower right') #绘制图例
# plt.show()


# import matplotlib.pyplot as plt
# from pylab import mpl
# """一元线性拟合
# 采用的拟合数据为xi=1,2,3,4,5,6,7
# 对应的相应函数值yi=0.5,2.5,2,4,3.5,6,5.5
# """
 
# x = [1, 2, 3, 4, 5, 6, 7,8]
# y = [0.5, 2.5, 2, 4, 3.5, 6, 5.5,3]
 
 
# """完成拟合曲线参数计算"""
# def liner_fitting(data_x,data_y):
#       size = len(data_x)
#       i=0
#       sum_xy=0
#       sum_y=0
#       sum_x=0
#       sum_sqare_x=0
#       average_x=0
#       average_y=0
#       while i<size:
#           sum_xy+=data_x[i]*data_y[i]
#           sum_y+=data_y[i]
#           sum_x+=data_x[i]
#           sum_sqare_x+=data_x[i]*data_x[i]
#           i+=1
#       average_x=sum_x/size
#       average_y=sum_y/size
#       return_k=(size*sum_xy-sum_x*sum_y)/(size*sum_sqare_x-sum_x*sum_x)
#       return_b=average_y-average_x*return_k
#       return [return_k,return_b]
# print( liner_fitting(x,y))
 
# """完成完后曲线上相应的函数值的计算"""
# def calculate(data_x,k,b):
#     datay=[]
#     for x in data_x:
#         datay.append(k*x+b)
#     return datay
 
 
# """完成函数的绘制"""
# def draw(data_x,data_y_new,data_y_old):
#     plt.plot(data_x,data_y_new,label="拟合曲线",color="black")
#     plt.scatter(data_x,data_y_old,label="离散数据")
#     mpl.rcParams['font.sans-serif'] = ['SimHei']
#     mpl.rcParams['axes.unicode_minus'] = False
#     plt.title("一元线性拟合数据")
#     plt.legend(loc="upper left")
#     plt.show()
 
 
# parameter = liner_fitting(x,y)
# draw_data = calculate(x,parameter[0],parameter[1])
# draw(x,draw_data,y)