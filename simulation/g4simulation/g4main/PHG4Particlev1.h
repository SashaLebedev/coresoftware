#ifndef PHG4PARTICLEV1_H__
#define PHG4PARTICLEV1_H__

#include "PHG4Particle.h"

class PHG4Particlev1: public PHG4Particle
{
 public:
  PHG4Particlev1();
  PHG4Particlev1(const std::string &name, const int pid, const double px, const double py, const double pz);
  PHG4Particlev1(const PHG4Particle *in);

  virtual ~PHG4Particlev1() {}

  int get_pid() const {return fpid;}
  std::string get_name() const {return fname;}
  double get_px() const {return fpx;}
  double get_py() const {return fpy;}
  double get_pz() const {return fpz;}

  void set_name(const std::string &name) {fname=name;}
  void set_pid(const int i) {fpid=i;}
  void set_px(const double x) {fpx = x;}
  void set_py(const double x) {fpy = x;}
  void set_pz(const double x) {fpz = x;}

  void identify(std::ostream& os = std::cout) const;

 protected:
  std::string fname;
  int fpid;
  double fpx,fpy,fpz;
  ClassDef(PHG4Particlev1,1)
};

#endif
