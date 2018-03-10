---
layout: post
title: 图书管理系统
tags: 小程序
---
### 项目要求说明：



> * 1、问题描述
图书管理系统业务活动包括：对新书的采编入库、清除库存、图书借阅、图书归还等。设计一个图书管理系统，借助于2-3树结构实现上述业务活动。

> * 2、基本要求：

>
```
（1).每种图书的登记内容包括：书号、书名、作者、定价、出版社、出版时间、现存量、总存量等；
（2）.演示程序以用户和计算机的对话方式进行,在计算机终端上显示“提示信息”之后，由用户在键盘上输入演示程序中规定的运算命令，相应的输入数据和运算结果显示在后面。该演示系统，没有使用文件，全部数据放在内存存放。四项基本业务都以书号为关键字进行的，采用了B树（2-3树）对书号建立索引，以提高效率。
（3）该系统系统实现的基本功能包括：
      1. 采编入库：   --凹入表的形式展示b树的形状

      2. 清除库存：  --实现某本书的信息删除--  凹入表的形式展示b树的形状

      3. 图书借阅：   书名查找 >0, 登记  用户账号，姓名 ->借出 --系统抓取借阅时间，计算归还时间

      4. 图书预约： 	=0 记录->姓名 账号    --当前的预约时间，取书时间

      5. 图书归还：	注销借阅者信息，变化现存量

      6. 作者专区：  作者名字 - > 显示全部著作信息

      7. 图书信息： 书号-查找-> 基本信息， 借阅信息，id,名字,借阅时间，归还时间预约信息，（可找到全部图书的基本信息）
```
> * 3·如果B树实现有困难，也可以考虑用二叉平衡树，但不得以其它数据结构作为主体模拟任务实现。


### 大纲设计：


>  * 图书管理系统流程图：
> 1.界面控制：
    图书管理系统流程图:   ![登录界面与各个界面之间的切换](/images/posts/2018-03-09/图书管理系统流程图.png)

> 2.主要功能的详细设计流程图：
       新书入库：
       ![新书入库](/images/posts/2018-03-09/图书采集.png)
         清除库存：![清除库存](/images/posts/2018-03-09/清除库存.png)
          图书借阅：![图书借阅](/images/posts/2018-03-09/图书借阅.png)
          图书预约：![图书预约](/images/posts/2018-03-09/图书预约.png)
          图书归还：![归还图书](/images/posts/2018-03-09/图书归还.png)
          图书信息：![图书信息](/images/posts/2018-03-09/图书信息.png)
          作者专区: ![作者专区](/images/posts/2018-03-09/作者专区.png)



### 难点剖析：


 > 1. 整个项目的难点在于B树插入与删除，完成B树的插入与删除后，后续的主要功能的设计就变得非常简单了，就不一一说明了。
 > 2. [B树请点这](https://16110905035.github.io/2018/03/Btree/)



###  程序设计的整体效果：


 > 1. 欢迎界面：![欢迎界面](/images/posts/2018-03-09/欢迎界面.png)
 > 2. 初始登录界面：![登录界面1](/images/posts/2018-03-09/初始登录界面.png)(ps:每次登录有三次输错密码的机会)
 ![管理员界面](/images/posts/2018-03-09/管理员界面.jpg)
 ![用户登录界面](/images/posts/2018-03-09/用户登录界面.png)

 > 2. 用户界面：![用户界面](/images/posts/2018-03-09/用户界面.png)

### ps:
> 当然还是有一些图片的，只不过就没有做到排版效果不是很好，所以我也就不摆出来了(0_0)!




###  不足之处：


 > * 未实现书号与书籍名称之间的一一对应关系，该图书管理系统只能使用书号进行查找。
 > * 书籍存储存在容量问题，限制条件为数组的长度
 > * 用户、管理员登录界面功能不够完善，未存取用户的个人信息，未能实现多用户、多个管理员之间的登录与注销。



### 项目的特点：
 > 1. 增加开始动画界面与页面转载界面。
> 2. 在各个操作主界面添加了时间显示。
> 3. 将程序分为两个模块，用户模块，管理员模块，模块清晰，内容明确。
> 4. 管理员界面设置了密码，避免了非管理员用户登录。
> 5. 用户界面加入跳转注册系统，可以随时注册。
> 6. 在管理员模块用户模块添加可以作者专区，与图书信息专区。


***代码我放在github仓库了，需要请戳这-->[github](https://github.com/16110905035/Books-Management-System-BMS-)***

注意： 以上内容，作者一字一句码出来的，纯属不易，欢迎大家转载，转载是还请您表明出处。另外如果我有侵权行为，请在下方留言，确认后我会及时撤销相应内容，谢谢大家！

 PS:欢迎大家来到我的小站,和我一起记录属于我们自己的大学！
