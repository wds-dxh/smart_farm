import { createApp } from 'vue'
import './style.css'
import App from './App.vue'
//导入element-plus
import ElementPlus from 'element-plus'
import 'element-plus/dist/index.css'
import router from './router'
//注册所有图标
import * as ElementPlusIconsVue from '@element-plus/icons-vue'


const app = createApp(App)
for (const [key, component] of Object.entries(ElementPlusIconsVue)) {
  app.component(key, component)
}
app.use(router)
app.use(ElementPlus).mount('#app')      //要最后挂载，否则会出现样式覆盖问题