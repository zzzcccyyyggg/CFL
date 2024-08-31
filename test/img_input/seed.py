import os
import random
from PIL import Image

# 创建目录保存生成的图片
output_dir = "random_images"
os.makedirs(output_dir, exist_ok=True)

# 生成 100 个随机 PNG 图片
for i in range(100):
    # 随机图片的宽度和高度
    width, height = random.randint(50, 300), random.randint(50, 300)
    
    # 创建一个随机颜色的图片
    image = Image.new('RGB', (width, height), (random.randint(0, 255), random.randint(0, 255), random.randint(0, 255)))
    
    # 随机填充图片内容
    for x in range(width):
        for y in range(height):
            image.putpixel((x, y), (random.randint(0, 255), random.randint(0, 255), random.randint(0, 255)))

    # 保存图片
    image_path = os.path.join(output_dir, f"image_{i+1}.png")
    image.save(image_path)

print(f"Generated 100 random PNG images in {output_dir} directory.")