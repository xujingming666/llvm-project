from tvm.driver import tvmc

model = tvmc.load('/share/home/xujing8/swin_tiny.pt', model_format='pytorch', shape_dict={"img":[1920,1080,3]})



