from flask import Flask, Response, render_template
import cv2
from use_model import ImageClassifier

app = Flask(__name__)

# 加载类别标签
with open('class_names.txt', 'r') as f:
    class_names = [line.strip() for line in f]

# 创建 ImageClassifier 实例
classifier = ImageClassifier('best_mobilenet_model.pth', class_names)

# # 打开摄像头
# camera = cv2.VideoCapture(0)

def generate_frames():
    while True:
        # 读取摄像头画面
        # success, frame = camera.read()
        # if not success:
        #     break
        frame = cv2.imread('./test.png')
        if frame is None:
            break
        
        # 进行图像分类
        prediction = classifier.classify_image(frame)
        
        # 在图像上绘制预测结果
        cv2.putText(frame, f'Prediction: {prediction}', (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
        
        # 编码图像为 JPEG 格式
        _, buffer = cv2.imencode('.jpg', frame)
        frame = buffer.tobytes()
        
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/video_feed')
def video_feed():
    return Response(generate_frames(),
                    mimetype='multipart/x-mixed-replace; boundary=frame')

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5001)
