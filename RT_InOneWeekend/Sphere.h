#pragma once
#include "Hitable.h"

class Sphere : public Hitable {
public:
	Sphere() {}
	Sphere(Vector3 cen, float r, Material *m) : center(cen), radius(r), mat_ptr(m) {};
	virtual bool hit(const Ray& r, float tmin, float tmax, hit_record& rec) const;

	Vector3 center;
	float radius;
	Material *mat_ptr;
};

class MovingSphere : public Hitable {
public:
	MovingSphere() {}
	MovingSphere(Vector3 cen0, Vector3 cen1, float t0, float t1, float r, Material *m)
		: center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), mat_ptr(m)
		{};
	virtual bool hit(const Ray& r, float tmin, float tmax, hit_record& rec) const;
	//virtual bool bounding_box(float t0, float t1, aabb& box) const;
	Vector3 center(float time) const;
	
	Vector3 center0, center1;
	float time0, time1;
	float radius;
	Material *mat_ptr;
};

bool Sphere::hit(const Ray& r, float t_min, float t_max, hit_record& rec) const {
	Vector3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = dot(oc, r.direction());
	float c = dot(oc, oc) - radius*radius;
	float discriminant = b*b - a*c;
	if (discriminant > 0) {
		float temp = (-b - sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = mat_ptr;
			return true;
		}
		temp = (-b + sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = mat_ptr;
			return true;
		}
	}
	return false;
}

Vector3 MovingSphere::center(float time) const {
	return center0 + ((time - time0) / (time1 - time0))*(center1 - center0);
}

bool MovingSphere::hit(const Ray& r, float t_min, float t_max, hit_record& rec) const {
	Vector3 oc = r.origin() - center(r.time());
	float a = dot(r.direction(), r.direction());
	float b = dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - a * c;
	if (discriminant > 0) {
		float temp = (-b - sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center(r.time())) / radius;
			rec.mat_ptr = mat_ptr;
			return true;
		}
		temp = (-b + sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center(r.time())) / radius;
			rec.mat_ptr = mat_ptr;
			return true;
		}
	}
	return false;
}