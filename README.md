# FuncBranchTool
A linux PIN tool for monitoring branches and functions!!

1.Create a "FuncBranchTool" folder in your Intel PIN tool path.

2.Copy whole repo content to "FuncBranchTool".

3.Put "network.h" in the following path:
      your-pin-path/source/include/pin
      
4.Run "make" command.

5.Use this tool by the following command to run the server:
      pin -t obj-intel64/server.so -- target/targetapp
      
6.Go to client folder and run "./clinet".

7.See the information in "branch_function_info.txt" file in client folder.
