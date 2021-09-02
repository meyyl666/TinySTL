git init       //初始化
git config --global user.email "you@example.com"  //配置用户信息
git config --global user.name "you name"""

git add .      //添加到缓冲区
git status     //查看状态
git commit -m "..."  //生成新版本
git log        //查看版本

//新版本回滚到之前的版本（已控制版本）
git reset --hard 版本号

//新版本回滚到之前的版本（变动版本）
git reset --mix  版本号

//暂存区回滚到之前变动版本
git reset HEAD

//变动版本回滚到已控制文件
git checkout

//回滚之后的版本
git reflog
git reset --hard 版本号

git branch   //查看分支

git branch 分支名    //创建分支

git checkout 分支名  //切换分支

git merge 分支名     //合并分支

git branch -d 分支名  //删除分支


git remote add origin 地址     //给地址取别名 origin
git push [-u] origin master    //推送代码  -u代表默认地址   origin 仓库地址    master 分支名
git pull origin dev            //更新代码  等同于 git fetch origin dev     git merge origin/dev(git rebase origin/dev)
git clone 地址                 //克隆代码



rebase  //将多个记录整合成一个记录  建议不要和已推送到仓库的版本合并

git rebase -i  版本号  (区间合并)
git rebase -i  HEAD~3 （从当前版本到最近的3条记录）
  


快速解决冲突
安装 beyond compare  
在git中配置
   git config --local merge.tool bc3 //命名
   git config --local mergetool.path '路径'
   git config --local mergetool.keepBackup false  //不用保留源文件
应用
   git mergetool 
