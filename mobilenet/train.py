'''
Author: wds-dxh wdsnpshy@163.com
Date: 2024-07-19 12:49:09
LastEditors: wds-dxh wdsnpshy@163.com
LastEditTime: 2024-07-19 12:49:19
FilePath: \mobilenet\main.py
Description: 使用mobilenet进行图像分类
微信: 15310638214 
邮箱：wdsnpshy@163.com 
Copyright (c) 2024 by ${wds-dxh}, All Rights Reserved. 
'''
import torch
import torch.nn as nn
import torch.optim as optim
from torchvision import datasets, transforms, models
from torch.utils.data import DataLoader
import os

# 设置设备
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

# 定义数据转换
transform = transforms.Compose([
    transforms.Resize((224, 224)),
    transforms.ToTensor(),
    transforms.Normalize(mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225])
])

# 加载数据集
train_dataset = datasets.ImageFolder(root=r'D:\OneDrive\workspace\now\Smart farm\mobilenet\dataset', transform=transform)
train_loader = DataLoader(train_dataset, batch_size=32, shuffle=True)

val_dataset = datasets.ImageFolder(root=r'D:\OneDrive\workspace\now\Smart farm\mobilenet\dataset', transform=transform)
val_loader = DataLoader(val_dataset, batch_size=32, shuffle=False)


# 保存类别标签到文件
class_names = train_dataset.classes
with open('class_names.txt', 'w') as f:
    for class_name in class_names:
        f.write(f"{class_name}\n")

# 定义MobileNet模型
model = models.mobilenet_v2(pretrained=True)
model.classifier[1] = nn.Linear(model.classifier[1].in_features, len(train_dataset.classes))
model = model.to(device)



# 定义损失函数和优化器
criterion = nn.CrossEntropyLoss()
optimizer = optim.Adam(model.parameters(), lr=0.001)

# 训练函数
def train(model, train_loader, criterion, optimizer, device):
    model.train()
    running_loss = 0.0
    for inputs, labels in train_loader:
        inputs, labels = inputs.to(device), labels.to(device)

        optimizer.zero_grad()
        outputs = model(inputs)
        loss = criterion(outputs, labels)
        loss.backward()
        optimizer.step()
        
        running_loss += loss.item() * inputs.size(0)
        
    epoch_loss = running_loss / len(train_loader.dataset)
    return epoch_loss

# 验证函数
def validate(model, val_loader, criterion, device):
    model.eval()
    running_loss = 0.0
    corrects = 0
    with torch.no_grad():
        for inputs, labels in val_loader:
            inputs, labels = inputs.to(device), labels.to(device)
            outputs = model(inputs)
            loss = criterion(outputs, labels)
            running_loss += loss.item() * inputs.size(0)
            _, preds = torch.max(outputs, 1)
            corrects += torch.sum(preds == labels.data)
    
    epoch_loss = running_loss / len(val_loader.dataset)
    accuracy = corrects.double() / len(val_loader.dataset)
    return epoch_loss, accuracy

# 保存模型函数
def save_model(model, path):
    torch.save(model.state_dict(), path)

# 加载模型函数
def load_model(model, path):
    model.load_state_dict(torch.load(path))
    model.to(device)
    model.eval()

# 主训练循环
num_epochs = 10
best_acc = 0.0

for epoch in range(num_epochs):
    train_loss = train(model, train_loader, criterion, optimizer, device)
    val_loss, val_acc = validate(model, val_loader, criterion, device)
    
    print(f'Epoch {epoch+1}/{num_epochs}, Train Loss: {train_loss:.4f}, Val Loss: {val_loss:.4f}, Val Acc: {val_acc:.4f}')
    
    # 保存最好的模型
    if val_acc > best_acc:
        best_acc = val_acc
        save_model(model, 'best_mobilenet_model.pth')

# 加载最好的模型
load_model(model, 'best_mobilenet_model.pth')
