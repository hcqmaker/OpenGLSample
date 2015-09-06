#include "gui.h"
#include "assert.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


namespace Simple
{

    std::vector<Window*> renderWins;
    Graphic* gp = new Graphic;

    const unsigned int TRANGLES_MAX_NUM = 5000;
    struct st_graphic_buffer_t
    {
        unsigned int num;           // num of vertex
        unsigned int tex;
        vec3_t vertexs[TRANGLES_MAX_NUM  * 3];
        vec2_t uvs[TRANGLES_MAX_NUM * 3];
        vec3_t normals[TRANGLES_MAX_NUM * 3];
        vec3_t colors[TRANGLES_MAX_NUM * 3];

        unsigned int num_trangles;
        unsigned int indexs[TRANGLES_MAX_NUM * 3];
    };

    st_graphic_buffer_t T;

    //===========================================================
    //
    graphic_buffer_t* create_rd_buffer()
    {
        graphic_buffer_t* p = new graphic_buffer_t;
        memset(p, 0, sizeof(graphic_buffer_t));
        return p;
    }

    //===========================================================
    //
    void gpc_bind_texture(unsigned int texturId)
    {
        T.tex = texturId;
    }
    //===========================================================
    //
    unsigned int gpc_get_texture_id()
    {
        return T.tex;
    }
    //===========================================================
    //
    unsigned int gpc_get_index()
    {
        return T.num;
    }

    //===========================================================
    //
    void gpc_push_vertex(float x, float y, float z)
    {
        T.vertexs[T.num][0] = x;
        T.vertexs[T.num][1] = y;
        T.vertexs[T.num][2] = z;
    }

    //===========================================================
    //
    void gpc_push_uv(float u, float v)
    {
        T.uvs[T.num][0] = u;
        T.uvs[T.num][1] = v;
    }

    void gpc_push_color(float r, float g, float b)
    {
        T.colors[T.num][0] = r;
        T.colors[T.num][1] = g;
        T.colors[T.num][2] = b;
    }

    //===========================================================
    //
    void gpc_push_normal(float x, float y, float z)
    {
        T.normals[T.num][0] = x;
        T.normals[T.num][1] = y;
        T.normals[T.num][2] = z;
    }

    //===========================================================
    //
    void gpc_push_trangles_index(int v0, int v1, int v2)
    {
        T.indexs[T.num * 3] = v0;
        T.indexs[T.num * 3 + 1] = v1;
        T.indexs[T.num * 3 + 2] = v2;
        T.num_trangles ++;
    }

    //===========================================================
    //
    void gpc_push_next()
    {
        T.num ++;
        assert(T.num < (TRANGLES_MAX_NUM * 3));
    }

    //===========================================================
    //
    void gpc_end_buff(graphic_buffer_t* t)
    {
        t->num_trangles = T.num_trangles;

        t->vertexs = new vec3_t[T.num];
        t->uvs = new vec2_t[T.num];
        t->normals = new vec3_t[T.num];
        t->colors = new vec3_t[T.num];

        t->indexs = new unsigned int[T.num_trangles];

        memcpy(t->vertexs, T.vertexs, sizeof(vec3_t) * T.num);
        memcpy(t->uvs, T.uvs, sizeof(vec2_t) * T.num);
        memcpy(t->normals, T.normals, sizeof(vec3_t) * T.num);
        memcpy(t->colors, T.colors, sizeof(vec3_t) * T.num);

        memcpy(t->indexs, T.indexs, sizeof(unsigned int) * T.num_trangles);
    }

    //===========================================================
    //
    void gpc_render(graphic_buffer_t* t)
    {
        glBindTexture(GL_TEXTURE_2D, t->tex);

        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, t->vertexs);

        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLAT, 0, t->uvs);

        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(3, GL_FLOAT, 0, t->colors);

        glDrawElements(GL_QUADS, t->num_trangles * 3, GL_UNSIGNED_INT, t->vertexs);

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
    }

    //===========================================================
    //
    void delete_gpc_buffer(graphic_buffer_t* t)
    {
        if (t->vertexs)
            delete[] t->vertexs;
        if (t->uvs)
            delete[] t->uvs;
        if (t->normals)
            delete[] t->normals;
        if (t->colors)
            delete[] t->colors;
        if (t->indexs)
            delete[] t->indexs;
        delete t;
    }


    //===========================================================
    //
    Graphic::Graphic()
    {

    }

    //===========================================================
    //
    Graphic::~Graphic()
    {
        vector<graphic_buffer_t*>::iterator i = mGraphics.begin();
        for (; i != mGraphics.end(); ++i)
            delete_gpc_buffer(*i);
        mGraphics.clear();
    }

    //===========================================================
    //
    void Graphic::setView(float w, float h)
    {
        mWidth = w;
        mHeight = h;
    }

    //===========================================================
    //
    void Graphic::bindtextrue(unsigned int tex)
    {
        if (gpc_get_texture_id() > 0 && gpc_get_texture_id() != tex)
            finish();

        gpc_bind_texture(tex);
    }

    //===========================================================
    //
    unsigned int Graphic::getVertexIndex()
    {
        return gpc_get_index();
    }

    //===========================================================
    //
    void Graphic::vertex(float x, float y, float z)
    {
        gpc_push_vertex(x, y, z);
    }

    //===========================================================
    //
    void Graphic::uv(float u, float v)
    {
        gpc_push_uv(u, v);
    }

    //===========================================================
    //
    void Graphic::color(float r, float g, float b)
    {
        gpc_push_color(r, g, b);
    }

    //===========================================================
    //
    void Graphic::normal(float x, float y, float z)
    {
        gpc_push_normal(x, y, z);
    }

    //===========================================================
    //
    void Graphic::index(int v0, int v1, int v2)
    {
        gpc_push_trangles_index(v0, v1, v2);
    }

    //===========================================================
    //
    void Graphic::next_vertex()
    {
        gpc_push_next();
    }

    //===========================================================
    //
    void Graphic::finish()
    {
        graphic_buffer_t* t = new graphic_buffer_t;
        gpc_end_buff(t);
        mGraphics.push_back(t);
    }

    //===========================================================
    //
    void Graphic::beginDraw()
    {
        //save current attributes
		glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
		glPushAttrib(GL_ALL_ATTRIB_BITS);

		glPolygonMode(GL_FRONT, GL_FILL);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();

        float mHalfWidth = mWidth / 2;
        float mHalfHeight = mHeight / 2;
		//glOrtho(-g_gui_half_width * g_gui_ar, g_gui_half_width * g_gui_ar, -g_gui_half_height, g_gui_half_height, -1, 1);
		glOrtho(-mHalfWidth, mHalfWidth, -mHalfHeight, mHalfHeight, -1, 1);

		//glOrtho(-1, 1, -1, 1, -1, 1);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_FOG);
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		glDisable(GL_TEXTURE_GEN_R);

		//glFrontFace(GL_CW);
		//glCullFace(GL_BACK);
		//glEnable(GL_CULL_FACE);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glEnable(GL_TEXTURE_2D);
    }

    //===========================================================
    //
    void Graphic::draw()
    {
        vector<graphic_buffer_t*>::iterator i = mGraphics.begin();
        for (; i != mGraphics.end(); ++i)
            gpc_render((*i));
    }

    //===========================================================
    //
    void Graphic::endDraw()
    {
        glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);

		//restore former attributes
		glPopAttrib();
		glPopClientAttrib();
    }

    //===========================================================
    //
    void Graphic::render()
    {
        beginDraw();
        draw();
        endDraw();
    }
    //===========================================================
    //
    Window::Window()
    {
        mMouseDownWin = NULL;
    }
    //===========================================================
    //
    Window::Window(float x, float y, float w, float h)
    {
        mrdSize.w = w;
        mrdSize.h = h;
        mPosition.x = x;
        mPosition.y = y;
        mMouseDownWin = NULL;
    }
    //===========================================================
    //
    Window::~Window()
    {
        mMouseDownWin = NULL;
        std::vector<Window*>::iterator i = mChildren.begin();
        for (; i != mChildren.end(); ++i)
            delete (*i);
        mChildren.clear();
    }
    //===========================================================
    //
    void Window::removeChild(Window *win)
    {
        std::vector<Window*>::iterator i = mChildren.begin();
        for (; i != mChildren.end(); ++i)
        {
            if ((*i) == win)
            {
                mChildren.erase(i);
                break;
            }
        }
        if (mMouseDownWin == win)
            mMouseDownWin = NULL;
    }

    //===========================================================
    //
    bool Window::hitTest(float x, float y)
    {
        if (x < mPosition.x || x > mPosition.x + mrdSize.w)
            return false;
        if (y < mPosition.y || y > mPosition.y + mrdSize.h)
            return false;

        printf("hitTest true %f, %f\n", x, y);
        return true;
    }

    //===========================================================
    //
    int Window::mouseEvent(int button, int state, int x, int y)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
            if (state == GLFW_PRESS)
                mouseDown(x, y);
            else if (state == GLFW_RELEASE)
                mouseUp(x, y);
        }
        return 0;
    }
    //===========================================================
    //
    int Window::mouseDown(float x, float y)
    {
        if (!hitTest(x, y))
            return 0;

        std::vector<Window*>::iterator i = mChildren.begin();
        for (; i != mChildren.end(); ++i)
        {
            int ret = (*i)->mouseDown(x, y);
            if (ret > 0)
            {
                mMouseDownWin = (*i);
                return ret;
            }
        }

        mMouseDownWin = this;
        return 1;
    }
    //===========================================================
    //
    void Window::mouseMove(float x, float y)
    {
        if (mMouseDownWin)
            mMouseDownWin->mouseMove(x, y);
    }
    //===========================================================
    //
    void Window::mouseUp(float x, float y)
    {
        if (mMouseDownWin)
            mMouseDownWin->mouseUp(x, y);
    }
    //===========================================================
    //
    void Window::draw(float x, float y)
    {
        rdTexture &t = mTexture;
        rdPoint &p = mPosition;
        if (t.tex > 0)
        {
            rdSize &s = mrdSize;
            rdColor &c = mColor;

            unsigned int idx = gp->getVertexIndex();

            gp->bindtextrue(t.tex);

            // 1
            gp->vertex(p.x + x, p.y + y, 0);
            gp->color(c.r, c.g, c.b);
            gp->uv(t.u0, t.v0);
            gp->normal(0, 0, 1);
            gp->next_vertex();

            // 2
            gp->vertex(p.x + x + s.w, p.y + y, 0);
            gp->color(c.r, c.g, c.b);
            gp->uv(t.u1, t.v0);
            gp->normal(0, 0, 1);
            gp->next_vertex();

            // 3
            gp->vertex(p.x + x + s.w, p.y + y + s.h, 0);
            gp->color(c.r, c.g, c.b);
            gp->uv(t.u1, t.v1);
            gp->normal(0, 0, 1);
            gp->next_vertex();

            // 4
            gp->vertex(p.x + x, p.y + y + s.h, 0);
            gp->color(c.r, c.g, c.b);
            gp->uv(t.u0, t.v1);
            gp->normal(0, 0, 1);
            gp->next_vertex();

            gp->index(idx, idx + 1, idx + 2);
            gp->index(idx, idx + 2, idx + 3);
        }

        std::vector<Window*>::iterator i = mChildren.begin();
        for (; i != mChildren.end(); ++i)
            (*i)->draw(p.x + x, p.y + y);
    }

    //===========================================================
    //
    Button::Button(): Window(0, 0, 1, 1),mCallBack(NULL) {}
    //===========================================================
    //
    Button::Button(float x, float y, float w, float h): Window(x, y, w, h),mCallBack(NULL) {}

    //===========================================================
    //
    int Button::mouseDown(float x, float y)
    {
        int ret = Window::mouseDown(x, y);
        if (ret > 0 && mCallBack != NULL)
            mCallBack();
        return ret;
    }


    //===========================================================
    // 这个是很另类的做法需要注意的
    Label::Label():Window(0, 0, 1, 1){}
    //===========================================================
    //
    Label::Label(const std::string& str)
    : Window(0, 0, 1, 1), mString(str)
    {
    }

    //===========================================================
    // 这里无法叠在一起只有自己画了
    void Label::draw(float x, float y)
    {
        int sz = mString.size();
        const char* dt = mString.c_str();
        glPushMatrix();
        glTranslatef(x, y, 0);
        glScalef(0.1, 0.1, 0.1);
        glColor3f(mColor.r, mColor.g, mColor.b);
        //for (int i = 0; i < sz; ++i)
            //glutStrokeCharacter(GLUT_STROKE_ROMAN, dt[i]);
        glPopMatrix();
    }
    //===========================================================
    //

    Slider::Slider()
    : Window(0, 0, 1, 1)
    {
        mBar = new Window(0, 0, mrdSize.w, 1);
        mBar->setColor(rdColor(100, 255, 255));
        addChild(mBar);
    }
    //===========================================================
    //
    Slider::Slider(float x, float y, float w, float h):Window(x, y, w, h)
    {
        mBar = new Window(0, 0, mrdSize.w, 1);
        mBar->setColor(rdColor(100, 255, 255));
        addChild(mBar);
    }

    //===========================================================
    //
    int Slider::mouseDown(float x, float y)
    {
        int ret = Window::mouseDown(x, y);
        if (ret > 0)
        {
            if (mMouseDownWin == mBar)
            {

            }
        }
        return ret;
    }
    //===========================================================
    //
    void Slider::mouseMove(float x, float y)
    {

    }
    //===========================================================
    //
    void Slider::mouseUp(float x, float y)
    {

    }

}
