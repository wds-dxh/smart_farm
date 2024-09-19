import torch
from torchvision import transforms, models
import cv2
import numpy as np

class ImageClassifier:
    def __init__(self, model_path, class_names):
        """
        初始化 ImageClassifier 类
        
        :param model_path: 训练好的模型文件路径
        :param class_names: 类别名称列表
        """
        self.device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
        self.class_names = class_names
        
        # 定义数据转换
        self.transform = transforms.Compose([
            transforms.ToPILImage(),  # 将 NumPy 数组转换为 PIL 图像
            transforms.Resize((224, 224)),
            transforms.ToTensor(),
            transforms.Normalize(mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225])
        ])
        
        # 定义 MobileNet 模型
        self.model = models.mobilenet_v2(weights=None)
        self.model.classifier[1] = torch.nn.Linear(self.model.classifier[1].in_features, len(class_names))
        self.model = self.model.to(self.device)
        
        # 加载模型
        self.load_model(model_path)
    
    def load_model(self, path):
        """
        加载模型
        
        :param path: 模型文件路径
        """
        self.model.load_state_dict(torch.load(path))
        self.model.to(self.device)
        self.model.eval()
    
    def classify_image(self, image_np):
        """
        对图像进行分类
        
        :param image_np: OpenCV 读取到的图像数据（NumPy 数组）
        :return: 预测的类别名称
        """
        # 转换颜色空间从 BGR 到 RGB
        image_rgb = cv2.cvtColor(image_np, cv2.COLOR_BGR2RGB)
        
        # 进行图像预处理
        image = self.transform(image_rgb).unsqueeze(0).to(self.device)
        
        with torch.no_grad():
            outputs = self.model(image)
            _, predicted = torch.max(outputs, 1)
        
        return self.class_names[predicted.item()]

# 使用示例
if __name__ == "__main__":
    # 加载类别标签
    with open('class_names.txt', 'r') as f:
        class_names = [line.strip() for line in f]
    
    # 创建 ImageClassifier 实例
    classifier = ImageClassifier('best_mobilenet_model.pth', class_names)
    
    # 读取图像数据
    image_path = 'test.png'  # 替换为你想预测的图像路径
    image_np = cv2.imread(image_path)
    
    # 预测图像类别
    prediction = classifier.classify_image(image_np)
    print(f'Predicted class: {prediction}')
