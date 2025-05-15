# my_opengl_code

学习的教程： https://learnopengl-cn.github.io/      暂时止步 模型加载 (现在可以加载guitar backpack)。 高级不了一点点。


---

cpp文件 基本都是 cp 上一次的文件得到的。

---

为了让 emacs/LSP(clangd) 能找到 glag的 头文件。试了好几种，只有 .clang 是可行的。

---

`g++ glad.c -c -I../include`
`ar rcs libmyglad.a glad.o`

把 .a 复制到 my_opengl_code/learn 下

`g++ 01_test_run.cpp -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -I/mnt/239G/z_my_backup/opengl/my_linux/glad/include -lmyglad -L./`


---



glBindVertexArray(VAO) 和 glVertexAttribPointer 的语句顺序问题， 必须前面的先写。不然的话，不会显示图形。 
数据 和 操作分离， 所以 glVertexAttribPointer 没有指定 操作的数据， 这个数据 是由 glBindVertexArray 来指定的。  所以 必须 Bind 在前。
glBind 和 glEnable 无关， 但是 依赖传递了。

---

`g++ -c my_shader.cpp -I/mnt/239G/z_my_backup/opengl/my_linux/glad/include -lmyglad -L./`
`ar rs libmyglad.a my_shader.o`


---


16_xxxx.cpp

不理解为什么 官方的代码 没有 显示 光源。  不过 教程上的截图中有光源。

---


assimp / build.md

```bash
cd assimp
cmake CMakeLists.txt 
cmake --build .
```

不过不太懂， so文件 为什么不放 lib里，而是放bin里。

/mnt/239G/z_my_backup/opengl/assimp-5.4.3/    include bin



---

/mnt/239G/z_my_backup/opengl/3D_guitar_bag/


---


export LD_LIBRARY_PATH=/mnt/239G/z_my_backup/opengl/assimp-5.4.3/bin:$LD_LIBRARY_PATH

---


