#pragma once
#ifndef __TRACKBALL_H__
#define __TRACKBALL_H__
#include "cgmath.h"
#include "constant.h"

struct camera
{
	vec3	eye = vec3(80, 0, 160);
	vec3	at = vec3(0, 0, 0);
	vec3	up = vec3(0, 0, 1);
	mat4	view_matrix = mat4::look_at(eye, at, up);

	float	fovy = PI / 4.0f; // must be in radian
	float	aspect_ratio;
	float	dNear = 0.5f;
	float	dFar = 5000.0f;
	mat4	projection_matrix;
};
//*******************************************************************
camera	cam;

void camera_update(GLuint program, vec3 eye, vec3 at, vec3 up) {
	cam.view_matrix = mat4::look_at(eye, at, up);
	cam.aspect_ratio = window_size.x / float(window_size.y);
	cam.projection_matrix = mat4::perspective(cam.fovy, cam.aspect_ratio, cam.dNear, cam.dFar);

	GLuint uloc = glGetUniformLocation(program, "view_matrix");			if (uloc > -1) glUniformMatrix4fv(uloc, 1, GL_TRUE, cam.view_matrix);		// update the view matrix (covered later in viewing lecture)
	uloc = glGetUniformLocation(program, "projection_matrix");	if (uloc > -1) glUniformMatrix4fv(uloc, 1, GL_TRUE, cam.projection_matrix);	// update the projection matrix (covered later in viewing lecture)
}
struct trackball
{
	bool	b_tracking = false;
	float	scale;			// controls how much rotation is applied
	mat4	view_matrix0;	// initial view matrix
	vec2	m0;				// the last mouse position
	int button;
	int mods;
	vec3 eye, at, up;
	vec3 *eyep, *atp, *upp;
	vec3 new_eye, new_at, new_up;

	trackball( float rot_scale=1.0f ):scale(rot_scale){}
	bool is_tracking() const { return b_tracking; }
	void begin( const mat4& view_matrix, float x, float y, int _button ,int _mods,vec3* _eye, vec3* _at, vec3* _up)
	{
		b_tracking = true;			// enable trackball tracking
		m0 = vec2(x,y)*2.0f-1.0f;	// convert (x,y) in [0,1] to [-1,1]
		view_matrix0 = view_matrix;	// save current view matrix
		button = _button;
		mods = _mods;
		eyep = _eye;
		atp = _at;
		upp = _up;
		eye=new_eye = *_eye;
		at=new_at = *_at;
		up=new_up = *_up;
	}
	void end() { 
		*eyep = new_eye;
		*atp = new_at;
		*upp = new_up;
		b_tracking = false;
	}

	mat4 update_trackball( float x, float y )
	{
		// retrive the current mouse position
		vec2 m = vec2(x,y)*2.0f - 1.0f; // normalize xy

		// project a 2D mouse position to a unit sphere
		static const vec3 p0 = vec3(0, 0, 1.0f);//vec3(at.x,at.y,1.0f);	// reference position on sphere
		vec3 p1 = vec3(m.x-m0.x, m0.y-m.y,0);	// displacement with vertical swap
		if(!b_tracking||length(p1)<0.0001f) return view_matrix0;			// ignore subtle movement
		p1 *= scale;														// apply rotation scale
		p1 = vec3(p1.x,p1.y,sqrtf(max(0,1.0f-length2(p1)))).normalize();	// back-project z=0 onto the unit sphere

		// find rotation axis and angle
		// - right-mult of mat3 = inverse view rotation to world
		// - i.e., rotation is done in the world coordinates
		vec3 n = p0.cross(p1)*mat3(view_matrix0);

		float angle = asin( min(n.length(),0.999f) );
		//printf("<%lf>\r", angle);
		new_eye = eye * mat4::rotate(n.normalize(), angle);
		new_up = up * mat4::rotate(n.normalize(), angle);

		*eyep = new_eye;
		*atp = new_at;
		*upp = new_up;
		// return resulting rotation matrix
		//return view_matrix0 * mat4::rotate(n.normalize(),angle);
		return mat4::look_at(new_eye, new_at, new_up);
	}
	mat4 update_zoom(float x, float y)
	{
		vec2 m = vec2(x, y)*2.0f - 1.0f; // normalize xy
		vec3 p2 = vec3(0, m0.y - m.y, 0);	// displacement with vertical swap
		if (!b_tracking || length(p2) < 0.0001f) return view_matrix0;			// ignore subtle movement
		p2.y *= 0.5f;
		if (p2.y < -0.99f) p2.y =-0.99f;
		new_eye = vec3(eye.x+ (eye.x-at.x) * p2.y, eye.y + (eye.y-at.y)* p2.y, eye.z + (eye.z-at.z)*p2.y);

		*eyep = new_eye;
		*atp = new_at;
		*upp = new_up;
		return mat4::look_at(new_eye, new_at, new_up);
	}
	mat4 update_panning(float x, float y)
	{
		vec2 m = vec2(x, y)*2.0f - 1.0f; 
		vec3 p1 = vec3(m.x - m0.x, m0.y - m.y, 0);	
		if (!b_tracking || length(p1) < 0.0001f) return view_matrix0;			
		p1 *= scale*(eye-at).length();

		vec4 p2 = view_matrix0.inverse()*vec4(p1,0);
		vec3 p3 = vec3(p2.x, p2.y, p2.z);
		new_eye = eye - p3;
		new_at = at - p3;

		*eyep = new_eye;
		*atp = new_at;
		*upp = new_up;
		return mat4::look_at(new_eye,new_at,new_up) ;
	}
} ;
#endif // __TRACKBALL_H__

