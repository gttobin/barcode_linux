//============================================================================
// Name        : Barcode.cpp
// Author      : Gerard Tobin
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================



#include <stdio.h>

#include <future>
#include <string>
#include <iostream>
#include <time.h>
#include <exception>

#include<opencv2/opencv.hpp>

#include <stdio.h>
#include <math.h>
#include <vector>
#include <algorithm>
#include <list>
#include <thread>

using namespace std;





vector<long double> getScanline(string file_name,bool print)
{
	 vector<long double> scanline;



	 	 	 	 cv::Mat img;
				  img= imread(file_name, cv::IMREAD_COLOR);
				  resize(img,img,cv::Size(600, 400), cv::INTER_LINEAR);









                      if(print)
			          {
			              printf("\nscanline");



			          }


				      for( int j = 0; j < img.cols; j++)
				      {
				    	  cv::Vec3b bgrPixel = img.at<cv::Vec3b>(img.rows/2, j);
				    	//BGR not RGB.
				    	  //long double blue = bgrPixel[0];
				    	  //long double green = bgrPixel[1];
				    	  //long double red = bgrPixel[2];
				          //Y (x) = 0.299R(x) + 0.587G(x) + 0.114B(x)
				    	  long double intensity=(bgrPixel[2]*0.299+bgrPixel[1]*0.587+bgrPixel[0]*0.114)/255.0;
				          scanline.push_back(intensity);
				          if(print)
				          {
				              printf("\n( %d , %f )",j,(double)intensity);



				          }

				      }


				      double sum=0;
				      int counter=0;;
				      long double average=0.0;
				      for(unsigned int i=0;i<scanline.size();i++)
				      {

				    	 // if(scanline[i]>0.5)
				    	  //{
				    		  sum=sum+scanline[i];
				    		  counter++;
				    	  //}
				      }
				      average=sum/counter;
				      for(unsigned int i=0;i<scanline.size();i++)
				      {
				    	  if(scanline[i]>average)
				    	  {
				    	  scanline[i]=1.0;
				    	  }
				    	  else
                          {
                          scanline[i]=0.0;
                          }
				    	  if(scanline[i]>1.0)
				    	  {
				    		  scanline[i]=1.0;
				    	  }
				      }
				      int index_of_first_black_pixel=0;
				      int index_of_last_black_pixel=599;

				      for( int j = scanline.size()-1; j >= 0; j--)
				      {
                            if(scanline[j]<0.5)
                            {

                                index_of_last_black_pixel=j;
                                break;
                            }
				      }
		     	      scanline.erase( scanline.begin()+index_of_last_black_pixel+1, scanline.end()  );

                      for( int j = 0; j < scanline.size(); j++)
				      {
                            if(scanline[j]<0.5)
                            {
                                index_of_first_black_pixel=j;


                                break;
                            }
				      }
                     scanline.erase( scanline.begin(), scanline.begin() + index_of_first_black_pixel-1);




				      if(print)
				      {
                      printf("\nindex_of_first_black_pixel %d",index_of_first_black_pixel);

                      printf("\nindex_of_last_black_pixel %d",index_of_last_black_pixel);
				      printf("\nscanline length %llu",scanline.size());
				      }
				      return scanline;


}

vector<vector<int>> bars(vector<long double> scanline,bool print)
{
        double sum_intensity=0;
        double average_intensity=0;
        for(int i=0;i<scanline.size();i++)
        {
            sum_intensity=sum_intensity+scanline[i];
        }
        average_intensity=sum_intensity/scanline.size();
        if(print)
        {
           printf("\naverage_intensity %f",average_intensity);
        }

	    vector<int> binarizedPoints2;
	    for(unsigned  int i=0;i<scanline.size();i++)
	    {
	    	if(scanline[i]>=average_intensity)
	    	{
	    		binarizedPoints2.push_back(0);
	    	}
	    	else
	    	{
	    		binarizedPoints2.push_back(1);
	    	}
	    }

	    if(print)
	   	{
	    for(unsigned  int i=0;i<binarizedPoints2.size();i++)
	    {
	    	printf("\n(%d,%d)",i,binarizedPoints2[i]);
	    }
	    printf("\n binarizedPoints2 length %llu",binarizedPoints2.size());
	   	}
	    vector<vector<int>> bars;
	    int l=binarizedPoints2.size();

	    int i=0;
	    vector<int> templist0;
	    vector<int> templist1;
	    vector<int> templist;

	    while(i<=l)
	    {
	    	label0:
			if(binarizedPoints2[i]==0)
			{
				templist0.push_back(0);


				i++;
				goto label0;
			}
			else
			{
				bars.push_back(templist0);
				templist0.clear();
			}






			if(bars.size()>=60)
			{
				break;
			}
			label1:

			if(binarizedPoints2[i]==1)
			{
				templist1.push_back(1);

				i++;
				goto label1;
			}
			else
			{
				bars.push_back(templist1);
				templist1.clear();
			}




			if(bars.size()>=60)
			{
						break;
			}

	    }
	    if(print)
	   	{
	    printf("\nnumber of  bars   %llu",bars.size());
	   	}
	  for(unsigned  int i=l;i>0;i--)
	  {
		  if(binarizedPoints2[l]==binarizedPoints2[i])
		  {
			  templist.push_back(binarizedPoints2[l]);

		  }
		  else
		  {
			  break;
		  }
	  }
	  bars.push_back(templist);





	  return bars;
}

vector<int> leftPadding(vector<vector<int>> bars)
{

		return	bars.at(0);
}

vector<int> rightPadding(vector<vector<int>> bars)
{

		return	bars.at(60);
}

vector<vector<int>> leftGuard(vector<vector<int>> bars)
{

		vector<vector<int>> leftGuard;


		for(unsigned  int i=0;i<3;i++)
		{
		leftGuard.push_back(bars.at(i+1));
		}

		return	leftGuard;
}

vector<vector<int>> rightGuard(vector<vector<int>> bars)
{
			vector<vector<int>> rightGuard;


			for(unsigned int i=0;i<3;i++)
			{
			rightGuard.push_back(bars.at(i+57));
			}

			return	rightGuard;



}

vector<vector<int>> midGuard(vector<vector<int>> bars)
{
		vector<vector<int>> midGuard;


		for(unsigned int i=0;i<5;i++)
		{
			midGuard.push_back(bars.at(i+28));
		}

		return	midGuard;

}
vector<vector<vector<int>>> lhsAndRhsBars(vector<vector<int>> bars)
{
		vector<vector<vector<int>>> lhsAndRhsBars;
		vector<vector<int>> temp;

		for(unsigned int j=0;j<6;j++)
		{
			temp.clear();
		for(unsigned int i=0;i<4;i++)
		{
			temp.push_back(bars.at(i+j*4+4));
		}
			lhsAndRhsBars.push_back(temp);
		}

		for(unsigned int j=0;j<6;j++)
		{
			temp.clear();
		for(unsigned  int i=0;i<4;i++)
		{
			temp.push_back(bars.at(i+j*4+33));
		}
			lhsAndRhsBars.push_back(temp);
		}
		return	lhsAndRhsBars;

}

vector<vector<vector<int>>> lhsBars(vector<vector<int>> bars)
{
		vector<vector<vector<int>>> lhsBars;
		vector<vector<int>> temp;

		for(unsigned int j=0;j<6;j++)
		{
			temp.clear();
		for(unsigned int i=0;i<4;i++)
		{
			temp.push_back(bars.at(i+j*4+4));
		}
			lhsBars.push_back(temp);
		}


		return	lhsBars;

}

vector<vector<vector<int>>> rhsBars(vector<vector<int>> bars)
{
		vector<vector<vector<int>>> rhsBars;
		vector<vector<int>> temp;



		for(unsigned int j=0;j<6;j++)
		{
			temp.clear();
		for(unsigned  int i=0;i<4;i++)
		{
			temp.push_back(bars.at(i+j*4+33));
		}
			rhsBars.push_back(temp);
		}
		return	rhsBars;

}

double averageBarLength1(vector<vector<int>> bars)
{
		 vector<vector<int>> lGuard=leftGuard(bars);
		 vector<vector<int>> rGuard=rightGuard(bars);
		 vector<vector<int>> mGuard=midGuard(bars);
		 double avgBarLength1=0;

		avgBarLength1=(lGuard[0].size()+lGuard[2].size()+rGuard[0].size()+rGuard[2].size()+
				mGuard[1].size()+mGuard[3].size())/6;

		 return avgBarLength1;
}

double averageBarLength0(vector<vector<int>> bars)
{
	 	 	 vector<vector<int>> lGuard=leftGuard(bars);
			 vector<vector<int>> rGuard=rightGuard(bars);
			 vector<vector<int>> mGuard=midGuard(bars);
			 double avgBarLength0=0;

			avgBarLength0=(lGuard[1].size()+rGuard[1].size()+rGuard[2].size()+
					mGuard[0].size()+mGuard[2].size()+mGuard[4].size())/5;

			 return avgBarLength0;



}
long double maxDist_lhs(int j,long double avbl1,long double avbl0,vector<vector<vector<int>>> bars_lhs)
{
	long double avgBarLength0 = avbl0;
	long double  avgBarLength1 = avbl1;
	vector<vector<vector<int>>>  s = bars_lhs;

	vector<long double> r0L = {avgBarLength0*3, avgBarLength1*2, avgBarLength0,
	    avgBarLength1};
	vector<long double>  r0G = {avgBarLength0, avgBarLength1, avgBarLength0*2,
	    avgBarLength1*3};

	vector<long double>  r1L = {avgBarLength0*2, avgBarLength1*2, avgBarLength0*2,
	    avgBarLength1};
	vector<long double>  r1G = {avgBarLength0, avgBarLength1*2, avgBarLength0*2,
	    avgBarLength1*2};

	vector<long double>  r2L = {avgBarLength0*2, avgBarLength1, avgBarLength0*2,
	    avgBarLength1*2};
	vector<long double>  r2G = {avgBarLength0*2, avgBarLength1*2, avgBarLength0,
	    avgBarLength1*2};

	vector<long double>  r3L = {avgBarLength0, avgBarLength1*3, avgBarLength0, avgBarLength1};
	vector<long double>  r3G = {avgBarLength0, avgBarLength1, avgBarLength0*4, avgBarLength1};

	vector<long double>  r4L = {avgBarLength0, avgBarLength1, avgBarLength0*3,
	    avgBarLength1*2};
	vector<long double>  r4G = {avgBarLength0*2, avgBarLength1*3, avgBarLength0,
	    avgBarLength1};

	vector<long double>  r5L = {avgBarLength0, avgBarLength1*2, avgBarLength0*3,
	    avgBarLength1};
	vector<long double>  r5G = {avgBarLength0, avgBarLength1*3, avgBarLength0*2,
	    avgBarLength1};

	vector<long double>  r6L = {avgBarLength0, avgBarLength1, avgBarLength0, avgBarLength1*4};
	vector<long double>  r6G = {avgBarLength0*4, avgBarLength1, avgBarLength0, avgBarLength1};

	vector<long double>  r7L = {avgBarLength0, avgBarLength1*3, avgBarLength0,
	    avgBarLength1*2};
	vector<long double> r7G = {avgBarLength0*2, avgBarLength1, avgBarLength0*3,
	    avgBarLength1};

	vector<long double>  r8L = {avgBarLength0, avgBarLength1*2, avgBarLength0,
	    avgBarLength1*3};
	vector<long double>  r8G = {avgBarLength0*3, avgBarLength1, avgBarLength0*2,
	    avgBarLength1};

	vector<long double>  r9L = {avgBarLength0*3, avgBarLength1, avgBarLength0,
	    avgBarLength1*2};
	vector<long double>  r9G = {avgBarLength0*2, avgBarLength1, avgBarLength0,
	    avgBarLength1*3};



	  vector<vector<long double>>  r = {r0L, r0G, r1L,r1G, r2L, r2G, r3L, r3G,
	    r4L, r4G, r5L, r5G,  r6L, r6G, r7L, r7G, r8L,
	    r8G,  r9L, r9G};

	  long double total=0;

	  for(unsigned  int i=0;i<20;i++)
	  {
		  long double max=0;
		  max = pow(r.at(i).at(0)  - s.at(j).at(0).size(),2) +
				pow(r.at(i).at(1)  - s.at(j).at(1).size(),2) +
				pow(r.at(i).at(2)  - s.at(j).at(2).size(),2) +
				pow(r.at(i).at(3)  - s.at(j).at(3).size(),2);

		  if(max>=total)
		  {
			  total=max;
		  }
	  }
	return total;
}

long double maxDist_rhs(int j,long double avbl1,long double avbl0,vector<vector<vector<int>>> bars_rhs)
{
	long double avgBarLength0 = avbl0;
	long double  avgBarLength1 = avbl1;
	vector<vector<vector<int>>>  s = bars_rhs;


	vector<long double> r0R = {avgBarLength1*3, avgBarLength0*2, avgBarLength1,
	    avgBarLength0};

	vector<long double>  r1R = {avgBarLength1*2, avgBarLength0*2, avgBarLength1*2,
	    avgBarLength0};

	vector<long double>  r2R = {avgBarLength1*2, avgBarLength0, avgBarLength1*2,
	    avgBarLength0*2};

	vector<long double>  r3R = {avgBarLength1, avgBarLength0*3, avgBarLength1, avgBarLength0};

	vector<long double>  r4R = {avgBarLength1, avgBarLength0, avgBarLength1*3,
	    avgBarLength0*2};

	vector<long double>  r5R = {avgBarLength1, avgBarLength0*2, avgBarLength1*3,
	    avgBarLength0};

	vector<long double>  r6R = {avgBarLength1, avgBarLength0, avgBarLength1, avgBarLength0*4};

	vector<long double>  r7R = {avgBarLength1, avgBarLength0*3, avgBarLength1,
	    avgBarLength0*2};

	vector<long double>  r8R = {avgBarLength1, avgBarLength0*2, avgBarLength1,
	    avgBarLength0*3};

	vector<long double>  r9R = {avgBarLength1*3, avgBarLength0, avgBarLength1,
	    avgBarLength0*2};


	  vector<vector<long double>>  r = { r0R, r1R, r2R, r3R,
	     r4R,  r5R,  r6R, r7R, r8R,  r9R};

	  long double total=0;

	  for(unsigned  int i=0;i<10;i++)
	  {
		  long double max=0;
		  max = pow(r.at(i).at(0)  - s.at(j).at(0).size(),2) +
				pow(r.at(i).at(1)  - s.at(j).at(1).size(),2) +
				pow(r.at(i).at(2)  - s.at(j).at(2).size(),2) +
				pow(r.at(i).at(3)  - s.at(j).at(3).size(),2);

		  if(max>=total)
		  {
			  total=max;
		  }
	  }
	return total;
}

long double pdash_lhs(int i,int j,long double avbl1,long double avbl0,vector<vector<vector<int>>> bars_lhs)
{
	long double avgBarLength0 = avbl0;
	long double  avgBarLength1 = avbl1;
	vector<vector<vector<int>>>  s = bars_lhs;

	vector<long double> r0L = {avgBarLength0*3, avgBarLength1*2, avgBarLength0,
		    avgBarLength1};
		vector<long double>  r0G = {avgBarLength0, avgBarLength1, avgBarLength0*2,
		    avgBarLength1*3};

		vector<long double>  r1L = {avgBarLength0*2, avgBarLength1*2, avgBarLength0*2,
		    avgBarLength1};
		vector<long double>  r1G = {avgBarLength0, avgBarLength1*2, avgBarLength0*2,
		    avgBarLength1*2};

		vector<long double>  r2L = {avgBarLength0*2, avgBarLength1, avgBarLength0*2,
		    avgBarLength1*2};
		vector<long double>  r2G = {avgBarLength0*2, avgBarLength1*2, avgBarLength0,
		    avgBarLength1*2};

		vector<long double>  r3L = {avgBarLength0, avgBarLength1*3, avgBarLength0, avgBarLength1};
		vector<long double>  r3G = {avgBarLength0, avgBarLength1, avgBarLength0*4, avgBarLength1};

		vector<long double>  r4L = {avgBarLength0, avgBarLength1, avgBarLength0*3,
		    avgBarLength1*2};
		vector<long double>  r4G = {avgBarLength0*2, avgBarLength1*3, avgBarLength0,
		    avgBarLength1};

		vector<long double>  r5L = {avgBarLength0, avgBarLength1*2, avgBarLength0*3,
		    avgBarLength1};
		vector<long double>  r5G = {avgBarLength0, avgBarLength1*3, avgBarLength0*2,
		    avgBarLength1};

		vector<long double>  r6L = {avgBarLength0, avgBarLength1, avgBarLength0, avgBarLength1*4};
		vector<long double>  r6G = {avgBarLength0*4, avgBarLength1, avgBarLength0, avgBarLength1};

		vector<long double>  r7L = {avgBarLength0, avgBarLength1*3, avgBarLength0,
		    avgBarLength1*2};
		vector<long double> r7G = {avgBarLength0*2, avgBarLength1, avgBarLength0*3,
		    avgBarLength1};

		vector<long double>  r8L = {avgBarLength0, avgBarLength1*2, avgBarLength0,
		    avgBarLength1*3};
		vector<long double>  r8G = {avgBarLength0*3, avgBarLength1, avgBarLength0*2,
		    avgBarLength1};

		vector<long double>  r9L = {avgBarLength0*3, avgBarLength1, avgBarLength0,
		    avgBarLength1*2};
		vector<long double>  r9G = {avgBarLength0*2, avgBarLength1, avgBarLength0,
		    avgBarLength1*3};



		  vector<vector<long double>>  r = {r0L, r0G,r1L, r1G, r2L, r2G, r3L, r3G,
		    r4L, r4G, r5L, r5G,  r6L, r6G, r7L, r7G, r8L,
		    r8G,  r9L, r9G};


	  long double sum=0;
	  sum = pow(r.at(i).at(0)  - s.at(j).at(0).size(),2) +
	 				pow(r.at(i).at(1)  - s.at(j).at(1).size(),2) +
	 				pow(r.at(i).at(2)  - s.at(j).at(2).size(),2) +
	 				pow(r.at(i).at(3)  - s.at(j).at(3).size(),2);

	  long double total=0.0;
	  total = 1 - (sum/maxDist_lhs(j, avgBarLength1, avgBarLength0, s));
	  return total;
}

long double pdash_rhs(int i,int j,long double avbl1,long double avbl0,vector<vector<vector<int>>> bars_rhs)
{
	long double avgBarLength0 = avbl0;
	long double  avgBarLength1 = avbl1;
	vector<vector<vector<int>>>  s = bars_rhs;


	vector<long double> r0R = {avgBarLength1*3, avgBarLength0*2, avgBarLength1,
	    avgBarLength0};

	vector<long double>  r1R = {avgBarLength1*2, avgBarLength0*2, avgBarLength1*2,
	    avgBarLength0};

	vector<long double>  r2R = {avgBarLength1*2, avgBarLength0, avgBarLength1*2,
	    avgBarLength0*2};

	vector<long double>  r3R = {avgBarLength1, avgBarLength0*3, avgBarLength1, avgBarLength0};

	vector<long double>  r4R = {avgBarLength1, avgBarLength0, avgBarLength1*3,
	    avgBarLength0*2};

	vector<long double>  r5R = {avgBarLength1, avgBarLength0*2, avgBarLength1*3,
	    avgBarLength0};

	vector<long double>  r6R = {avgBarLength1, avgBarLength0, avgBarLength1, avgBarLength0*4};

	vector<long double>  r7R = {avgBarLength1, avgBarLength0*3, avgBarLength1,
	    avgBarLength0*2};

	vector<long double>  r8R = {avgBarLength1, avgBarLength0*2, avgBarLength1,
	    avgBarLength0*3};

	vector<long double>  r9R = {avgBarLength1*3, avgBarLength0, avgBarLength1,
	    avgBarLength0*2};


	  vector<vector<long double>>  r = { r0R, r1R, r2R, r3R,
	     r4R,  r5R,  r6R, r7R, r8R,  r9R};


	  long double sum=0;
	  sum = pow(r.at(i).at(0)  - s.at(j).at(0).size(),2) +
	 				pow(r.at(i).at(1)  - s.at(j).at(1).size(),2) +
	 				pow(r.at(i).at(2)  - s.at(j).at(2).size(),2) +
	 				pow(r.at(i).at(3)  - s.at(j).at(3).size(),2);

	  long double total=0.0;
	  total = 1 - (sum/maxDist_rhs(j, avgBarLength1, avgBarLength0, s));
	  return total;
}
long double p_lhs(int i,int j,double avbl1,double avbl0,vector<vector<vector<int>>> bars_lhs)
{
	long double avgBarLength0 = avbl0;
	long double  avgBarLength1 = avbl1;
	vector<vector<vector<int>>>  s = bars_lhs;

	vector<long double> r0L = {avgBarLength0*3, avgBarLength1*2, avgBarLength0,
			    avgBarLength1};
			vector<long double>  r0G = {avgBarLength0, avgBarLength1, avgBarLength0*2,
			    avgBarLength1*3};

			vector<long double>  r1L = {avgBarLength0*2, avgBarLength1*2, avgBarLength0*2,
			    avgBarLength1};
			vector<long double>  r1G = {avgBarLength0, avgBarLength1*2, avgBarLength0*2,
			    avgBarLength1*2};

			vector<long double>  r2L = {avgBarLength0*2, avgBarLength1, avgBarLength0*2,
			    avgBarLength1*2};
			vector<long double>  r2G = {avgBarLength0*2, avgBarLength1*2, avgBarLength0,
			    avgBarLength1*2};

			vector<long double>  r3L = {avgBarLength0, avgBarLength1*3, avgBarLength0, avgBarLength1};
			vector<long double>  r3G = {avgBarLength0, avgBarLength1, avgBarLength0*4, avgBarLength1};

			vector<long double>  r4L = {avgBarLength0, avgBarLength1, avgBarLength0*3,
			    avgBarLength1*2};
			vector<long double>  r4G = {avgBarLength0*2, avgBarLength1*3, avgBarLength0,
			    avgBarLength1};

			vector<long double>  r5L = {avgBarLength0, avgBarLength1*2, avgBarLength0*3,
			    avgBarLength1};
			vector<long double>  r5G = {avgBarLength0, avgBarLength1*3, avgBarLength0*2,
			    avgBarLength1};

			vector<long double>  r6L = {avgBarLength0, avgBarLength1, avgBarLength0, avgBarLength1*4};
			vector<long double>  r6G = {avgBarLength0*4, avgBarLength1, avgBarLength0, avgBarLength1};

			vector<long double>  r7L = {avgBarLength0, avgBarLength1*3, avgBarLength0,
			    avgBarLength1*2};
			vector<long double> r7G = {avgBarLength0*2, avgBarLength1, avgBarLength0*3,
			    avgBarLength1};

			vector<long double>  r8L = {avgBarLength0, avgBarLength1*2, avgBarLength0,
			    avgBarLength1*3};
			vector<long double>  r8G = {avgBarLength0*3, avgBarLength1, avgBarLength0*2,
			    avgBarLength1};

			vector<long double>  r9L = {avgBarLength0*3, avgBarLength1, avgBarLength0,
			    avgBarLength1*2};
			vector<long double>  r9G = {avgBarLength0*2, avgBarLength1, avgBarLength0,
			    avgBarLength1*3};



			  vector<vector<long double>>  r = {r0L, r0G,r1L, r1G, r2L, r2G, r3L, r3G,
			    r4L, r4G, r5L, r5G,  r6L, r6G, r7L, r7G, r8L,
			    r8G,  r9L, r9G};



	  long double total = 0;
	  for(unsigned int k = 0; k < 20; k++)
	  {
		  total = total + pdash_lhs(k, j, avgBarLength1, avgBarLength0, s);
	  }

	  long double  ret = pdash_lhs(i, j, avgBarLength1, avgBarLength0, s)/total;
	  return ret;



}


long double p_rhs(int i,int j,double avbl1,double avbl0,vector<vector<vector<int>>> bars_rhs)
{
	long double avgBarLength0 = avbl0;
	long double  avgBarLength1 = avbl1;
	vector<vector<vector<int>>>  s = bars_rhs;

	vector<long double> r0R = {avgBarLength1*3, avgBarLength0*2, avgBarLength1,
		    avgBarLength0};

		vector<long double>  r1R = {avgBarLength1*2, avgBarLength0*2, avgBarLength1*2,
		    avgBarLength0};

		vector<long double>  r2R = {avgBarLength1*2, avgBarLength0, avgBarLength1*2,
		    avgBarLength0*2};

		vector<long double>  r3R = {avgBarLength1, avgBarLength0*3, avgBarLength1, avgBarLength0};

		vector<long double>  r4R = {avgBarLength1, avgBarLength0, avgBarLength1*3,
		    avgBarLength0*2};

		vector<long double>  r5R = {avgBarLength1, avgBarLength0*2, avgBarLength1*3,
		    avgBarLength0};

		vector<long double>  r6R = {avgBarLength1, avgBarLength0, avgBarLength1, avgBarLength0*4};

		vector<long double>  r7R = {avgBarLength1, avgBarLength0*3, avgBarLength1,
		    avgBarLength0*2};

		vector<long double>  r8R = {avgBarLength1, avgBarLength0*2, avgBarLength1,
		    avgBarLength0*3};

		vector<long double>  r9R = {avgBarLength1*3, avgBarLength0, avgBarLength1,
		    avgBarLength0*2};


		  vector<vector<long double>>  r = { r0R, r1R, r2R, r3R,
		     r4R,  r5R,  r6R, r7R, r8R,  r9R};

	  long double total = 0;
	  for(unsigned int k = 0; k < 10; k++)
	  {
		  total = total + pdash_rhs(k, j, avgBarLength1, avgBarLength0, s);
	  }

	  long double  ret = pdash_rhs(i, j, avgBarLength1, avgBarLength0, s)/total;
	  return ret;



}
bool checkSum(vector<int> bc)
{
	vector<int> finalBarcode=bc;
	int evenSum=finalBarcode[10] + finalBarcode[8] + finalBarcode[6] +
		    finalBarcode[4] + finalBarcode[2] + finalBarcode[0];
	int oddSum=finalBarcode[11] + finalBarcode[9] + finalBarcode[7] +
		    finalBarcode[5] + finalBarcode[3] + finalBarcode[1];

	int	  checkSumDigit = (10 - (3*oddSum + evenSum)%10)%10;
	if(finalBarcode[12]==checkSumDigit)
	{ return true;}
	else
	{return false;}
}


void barcode(long double st,vector<vector<int>> bars,long double avbl1,long double avbl0,long double d)
{

	  long double delta = d;
	  long double step = st;

	  long double avgBarLength0 = avbl0;
	  long double  avgBarLength1 = avbl1;
	 vector<vector<int>>  s = bars;
	 vector<vector<vector<int>>>  s_lhs;
	 vector<vector<vector<int>>>  s_rhs;

	 s_lhs=lhsBars(s);

	 s_rhs=rhsBars(s);



	  vector<long double> maxProbabilities_lhs = {0, 0, 0, 0, 0, 0};
	  vector<int> maxProbabilitiesDigits_lhs = {0, 0, 0, 0, 0, 0};
	  vector<int> maxProbabilitiesIndices_lhs = {0, 0, 0, 0, 0, 0};
	  int maxProbIndex_lhs = 0;
	  long double maxProb_lhs=0;
	  vector<int> maxProbabilitiesDigits2_lhs = {0, 0, 0, 0, 0, 0};
	  vector<int> maxProbabilitiesIndices2_lhs = {0, 0, 0, 0, 0, 0};
	  vector<long double> maxProbabilities2_lhs = {0, 0, 0, 0, 0, 0};
	  vector<long double> finalProbList_lhs = {};
	  vector<double> parity_lhs = {};



	  vector<long double> maxProbabilities_rhs = {0, 0, 0, 0, 0, 0};
	  vector<int> maxProbabilitiesDigits_rhs = {0, 0, 0, 0, 0, 0};
	  vector<int> maxProbabilitiesIndices_rhs = {0, 0, 0, 0, 0, 0};
	  int maxProbIndex_rhs = 0;
	  long double maxProb_rhs=0;
	  vector<int> maxProbabilitiesDigits2_rhs = {0, 0, 0, 0, 0, 0};
	  vector<int> maxProbabilitiesIndices2_rhs = {0, 0, 0, 0, 0, 0};
	  vector<long double> maxProbabilities2_rhs = {0, 0, 0, 0, 0, 0,};
	  vector<long double> finalProbList_rhs = {};
	  vector<double> parity_rhs = {};
	  vector<int> finalbarCode={};


	  long double sigma = 1;
	  int digitNumber = -1;


	 for(long double c = (avgBarLength1 - delta); c <= (avgBarLength1 + delta); c = c + step)
	  {
	     for(long double e = (avgBarLength0 - delta); e <= (avgBarLength0 + delta); e = e + step)
		{


	      vector<long double> r0L ={c*3, e*2, c, e}; ;
	      vector<long double> r0G = {c, e, c*2, e*3};

	      vector<long double> r1L = {c*2, e*2, c*2, e};
	      vector<long double> r1G = {c, e*2, c*2, e*2};

	      vector<long double> r2L = {c*2, e, c*2, e*2};
	      vector<long double> r2G = {c*2, e*2, c, e*2};

	      vector<long double> r3L = {c, e*3, c, e};
	      vector<long double> r3G = {c, e, c*4, e};

	      vector<long double> r4L = {c, e, c*3, e*2};
	      vector<long double> r4G = {c*2, e*3, c, e};

	      vector<long double> r5L = {c, e*2, c*3, e};
	      vector<long double> r5G = {c, e*3, c*2, e};

	      vector<long double> r6L = {c, e, c, e*4};
	      vector<long double> r6G = {c*4, e, c, e};

	      vector<long double> r7L = {c, e*3, c, e*2};
	      vector<long double> r7G = {c*2, e, c*3, e};

	      vector<long double> r8L = {c, e*2, c, e*3};
	      vector<long double> r8G = {c*3, e, c*2, e};

	      vector<long double> r9L = {c*3, e, c, e*2};
	      vector<long double> r9G = {c*2, e, c, e*3};


	      vector<vector<long double>> r = {r0L, r0G, r1L, r1G,  r2L, r2G,  r3L, r3G,
	        r4L, r4G,  r5L, r5G,  r6L, r6G,  r7L, r7G, r8L,
	         r8G,  r9L, r9G};





	      for(int a = 0; a < 6; a++)
	      {

	       maxProb_lhs = 0;
	       maxProbIndex_lhs = 0;

	       for(int b = 0; b < 20; b++)
	       {

	        if(p_lhs(b, a, c, e, s_lhs) >= maxProb_lhs)
	        {
	        	maxProb_lhs = p_lhs(b, a, c, e, s_lhs);

	            maxProbIndex_lhs = b;
	        }

	       }


	       sigma = sigma*maxProb_lhs;





	       digitNumber = ((maxProbIndex_lhs - maxProbIndex_lhs%2)/2);




	       if(maxProb_lhs >= maxProbabilities_lhs[a])
	       {
	        maxProbabilities_lhs[a] = maxProb_lhs;
	        maxProbabilitiesDigits_lhs[a] = digitNumber;
	        maxProbabilitiesIndices_lhs[a] = maxProbIndex_lhs;
	       }



	       }



	      for(unsigned  int i = 0; i < 6; i++)
	      {
	       if(maxProbabilities_lhs[i] >= maxProbabilities2_lhs[i])
	       {
	        maxProbabilities2_lhs[i] = maxProbabilities_lhs[i];
	        maxProbabilitiesDigits2_lhs[i] = maxProbabilitiesDigits_lhs[i];
	        maxProbabilitiesIndices2_lhs[i] = maxProbabilitiesIndices_lhs[i];
	       }

	      }




		 }}



	 	 	 sigma=1;
	 for(long double c = (avgBarLength1 - delta); c <= (avgBarLength1 + delta); c = c + step)
		  {
		     for(long double e = (avgBarLength0 - delta); e <= (avgBarLength0 + delta); e = e + step)
			{



		      vector<long double> r0R = {e*3, c*2, e, c};

		      vector<long double> r1R = {e*2, c*2, e*2, c};

		      vector<long double> r2R = {e*2, c, e*2, c*2};

		      vector<long double> r3R = {e, c*3, e, c};

		      vector<long double> r4R = {e, c, e*3, c*2};

		      vector<long double> r5R = {e, c*2, e*3, c};

		      vector<long double> r6R = {e, c, e, c*4};

		      vector<long double> r7R = {e, c*3, e, c*2};

		      vector<long double> r8R = {e, c*2, e, c*3};

		      vector<long double> r9R = {e*3, c, e, c*2};

		      vector<vector<long double>> r = { r0R, r1R,  r2R,  r3R,
		         r4R,  r5R,  r6R,  r7R,  r8R,  r9R};





		      for(int a = 0; a < 6; a++)
		      {

		       maxProb_rhs = 0;
		       maxProbIndex_rhs = 0;

		       for(int b = 0; b < 10; b++)
		       {

		        if(p_rhs(b, a, c, e, s_rhs) >= maxProb_rhs)
		        {
		        	maxProb_rhs = p_rhs(b, a, c, e, s_rhs);

		            maxProbIndex_rhs = b;
		        }

		       }


		       sigma = sigma*maxProb_rhs;





		       digitNumber = maxProbIndex_rhs;



		       if(maxProb_rhs >= maxProbabilities_rhs[a])
		       {
		        maxProbabilities_rhs[a] = maxProb_rhs;
		        maxProbabilitiesDigits_rhs[a] = digitNumber;
		        maxProbabilitiesIndices_rhs[a] = maxProbIndex_rhs;
		       }



		       }



		      for(unsigned  int i = 0; i < 6; i++)
		      {
		       if(maxProbabilities_rhs[i] >= maxProbabilities2_rhs[i])
		       {
		        maxProbabilities2_rhs[i] = maxProbabilities_rhs[i];
		        maxProbabilitiesDigits2_rhs[i] = maxProbabilitiesDigits_rhs[i];
		        maxProbabilitiesIndices2_rhs[i] = maxProbabilitiesIndices_rhs[i];
		       }

		      }




	//		 }}


	     vector<int> finalBarCode = {};
	     for(unsigned  int i = 0; i < 6; i++)
	     {
	        finalBarCode.push_back(maxProbabilitiesDigits_lhs[i]);
	     }
	     for(unsigned  int i = 0; i < 6; i++)
	     {
	    	  finalBarCode.push_back(maxProbabilitiesDigits_rhs[i]);
	     }


	     for(unsigned int i = 0; i < 6; i++)
	     {
	        if((maxProbabilitiesIndices_lhs[i])%2 == 0)
	        {
	          parity_lhs.push_back(1);

	        }
	        if((maxProbabilitiesIndices_lhs[i])%2 == 1)
	        {
	        	parity_lhs.push_back(0);

	        }


	     }


	       vector<vector<int>> m = {{1, 1, 1, 1, 1, 1}, {1, 1, 0, 1, 0, 0}, {1, 1, 0, 0, 1, 0}, {1,
	           1, 0, 0, 0, 1}, {1, 0, 1, 1, 0, 0}, {1, 0, 0, 1, 1, 0}, {1, 0,
	          0, 0, 1, 1}, {1, 0, 1, 0, 1, 0}, {1, 0, 1, 0, 0, 1}, {1, 0, 0, 1,
	           0, 1}};

	       int mNumber = -1;

	       for(int i = 0; i < 10; i++)
	       {
	         if(m.at(i).at(0) == parity_lhs.at(0) &&
	            m.at(i).at(1) == parity_lhs.at(1) &&
				 m.at(i).at(2) == parity_lhs.at(2) &&
				 m.at(i).at(3) == parity_lhs.at(3) &&
				 m.at(i).at(4) == parity_lhs.at(4) &&
				 m.at(i).at(5) == parity_lhs.at(5) )
	         {
	        	 mNumber = i ; break;
	         }
	       }
	       finalBarCode.insert(finalBarCode.begin() , mNumber);


	       printf("\nfinal Bar Code method 1  ");
	       for(unsigned int i=0;i<13;i++)
	        {
	    	   printf(" %d ",finalBarCode[i]);
	        }

	    	printf(checkSum(finalBarCode) ? "true" : "false");








}}
}




string barcode_rhs(long double st,vector<vector<int>> bars,long double avbl1,long double avbl0,long double d)
{

	  long double delta = d;
	  long double step = st;

	  long double avgBarLength0 = avbl0;
	  long double  avgBarLength1 = avbl1;
	 vector<vector<int>>  s = bars;
	 vector<vector<vector<int>>>  s_lhs;
	 vector<vector<vector<int>>>  s_rhs;

//	 s_lhs=lhsBars(s);

	 s_rhs=rhsBars(s);


/**
	  vector<long double> maxProbabilities_lhs = {0, 0, 0, 0, 0, 0};
	  vector<int> maxProbabilitiesDigits_lhs = {0, 0, 0, 0, 0, 0};
	  vector<int> maxProbabilitiesIndices_lhs = {0, 0, 0, 0, 0, 0};
	  int maxProbIndex_lhs = 0;
	  long double maxProb_lhs=0;
	  vector<int> maxProbabilitiesDigits2_lhs = {0, 0, 0, 0, 0, 0};
	  vector<int> maxProbabilitiesIndices2_lhs = {0, 0, 0, 0, 0, 0};
	  vector<long double> maxProbabilities2_lhs = {0, 0, 0, 0, 0, 0};
	  vector<long double> finalProbList_lhs = {};
	  vector<double> parity_lhs = {};
*/


	  vector<long double> maxProbabilities_rhs = {0, 0, 0, 0, 0, 0};
	  vector<int> maxProbabilitiesDigits_rhs = {0, 0, 0, 0, 0, 0};
	  vector<int> maxProbabilitiesIndices_rhs = {0, 0, 0, 0, 0, 0};
	  int maxProbIndex_rhs = 0;
	  long double maxProb_rhs=0;
	  vector<int> maxProbabilitiesDigits2_rhs = {0, 0, 0, 0, 0, 0};
	  vector<int> maxProbabilitiesIndices2_rhs = {0, 0, 0, 0, 0, 0};
	  vector<long double> maxProbabilities2_rhs = {0, 0, 0, 0, 0, 0,};
	  vector<long double> finalProbList_rhs = {};
	  vector<double> parity_rhs = {};
	  vector<int> finalbarCode={};


	  long double sigma = 1;
	  int digitNumber = -1;

/**
	 for(long double c = (avgBarLength1 - delta); c <= (avgBarLength1 + delta); c = c + step)
	  {
	     for(long double e = (avgBarLength0 - delta); e <= (avgBarLength0 + delta); e = e + step)
		{


	      vector<long double> r0L ={c*3, e*2, c, e}; ;
	      vector<long double> r0G = {c, e, c*2, e*3};

	      vector<long double> r1L = {c*2, e*2, c*2, e};
	      vector<long double> r1G = {c, e*2, c*2, e*2};

	      vector<long double> r2L = {c*2, e, c*2, e*2};
	      vector<long double> r2G = {c*2, e*2, c, e*2};

	      vector<long double> r3L = {c, e*3, c, e};
	      vector<long double> r3G = {c, e, c*4, e};

	      vector<long double> r4L = {c, e, c*3, e*2};
	      vector<long double> r4G = {c*2, e*3, c, e};

	      vector<long double> r5L = {c, e*2, c*3, e};
	      vector<long double> r5G = {c, e*3, c*2, e};

	      vector<long double> r6L = {c, e, c, e*4};
	      vector<long double> r6G = {c*4, e, c, e};

	      vector<long double> r7L = {c, e*3, c, e*2};
	      vector<long double> r7G = {c*2, e, c*3, e};

	      vector<long double> r8L = {c, e*2, c, e*3};
	      vector<long double> r8G = {c*3, e, c*2, e};

	      vector<long double> r9L = {c*3, e, c, e*2};
	      vector<long double> r9G = {c*2, e, c, e*3};


	      vector<vector<long double>> r = {r0L, r0G, r1L, r1G,  r2L, r2G,  r3L, r3G,
	        r4L, r4G,  r5L, r5G,  r6L, r6G,  r7L, r7G, r8L,
	         r8G,  r9L, r9G};





	      for(int a = 0; a < 6; a++)
	      {

	       maxProb_lhs = 0;
	       maxProbIndex_lhs = 0;

	       for(int b = 0; b < 20; b++)
	       {

	        if(p_lhs(b, a, c, e, s_lhs) >= maxProb_lhs)
	        {
	        	maxProb_lhs = p_lhs(b, a, c, e, s_lhs);

	            maxProbIndex_lhs = b;
	        }

	       }


	       sigma = sigma*maxProb_lhs;





	       digitNumber = ((maxProbIndex_lhs - maxProbIndex_lhs%2)/2);




	       if(maxProb_lhs >= maxProbabilities_lhs[a])
	       {
	        maxProbabilities_lhs[a] = maxProb_lhs;
	        maxProbabilitiesDigits_lhs[a] = digitNumber;
	        maxProbabilitiesIndices_lhs[a] = maxProbIndex_lhs;
	       }



	       }



	      for(unsigned  int i = 0; i < 6; i++)
	      {
	       if(maxProbabilities_lhs[i] >= maxProbabilities2_lhs[i])
	       {
	        maxProbabilities2_lhs[i] = maxProbabilities_lhs[i];
	        maxProbabilitiesDigits2_lhs[i] = maxProbabilitiesDigits_lhs[i];
	        maxProbabilitiesIndices2_lhs[i] = maxProbabilitiesIndices_lhs[i];
	       }

	      }




		 }}

*/

	 	 	 sigma=1;
	 for(long double c = (avgBarLength1 - delta); c <= (avgBarLength1 + delta); c = c + step)
		  {
		     for(long double e = (avgBarLength0 - delta); e <= (avgBarLength0 + delta); e = e + step)
			{



		      vector<long double> r0R = {e*3, c*2, e, c};

		      vector<long double> r1R = {e*2, c*2, e*2, c};

		      vector<long double> r2R = {e*2, c, e*2, c*2};

		      vector<long double> r3R = {e, c*3, e, c};

		      vector<long double> r4R = {e, c, e*3, c*2};

		      vector<long double> r5R = {e, c*2, e*3, c};

		      vector<long double> r6R = {e, c, e, c*4};

		      vector<long double> r7R = {e, c*3, e, c*2};

		      vector<long double> r8R = {e, c*2, e, c*3};

		      vector<long double> r9R = {e*3, c, e, c*2};

		      vector<vector<long double>> r = { r0R, r1R,  r2R,  r3R,
		         r4R,  r5R,  r6R,  r7R,  r8R,  r9R};





		      for(int a = 0; a < 6; a++)
		      {

		       maxProb_rhs = 0;
		       maxProbIndex_rhs = 0;

		       for(int b = 0; b < 10; b++)
		       {

		        if(p_rhs(b, a, c, e, s_rhs) >= maxProb_rhs)
		        {
		        	maxProb_rhs = p_rhs(b, a, c, e, s_rhs);

		            maxProbIndex_rhs = b;
		        }

		       }


		       sigma = sigma*maxProb_rhs;





		       digitNumber = maxProbIndex_rhs;



		       if(maxProb_rhs >= maxProbabilities_rhs[a])
		       {
		        maxProbabilities_rhs[a] = maxProb_rhs;
		        maxProbabilitiesDigits_rhs[a] = digitNumber;
		        maxProbabilitiesIndices_rhs[a] = maxProbIndex_rhs;
		       }



		       }



		      for(unsigned  int i = 0; i < 6; i++)
		      {
		       if(maxProbabilities_rhs[i] >= maxProbabilities2_rhs[i])
		       {
		        maxProbabilities2_rhs[i] = maxProbabilities_rhs[i];
		        maxProbabilitiesDigits2_rhs[i] = maxProbabilitiesDigits_rhs[i];
		        maxProbabilitiesIndices2_rhs[i] = maxProbabilitiesIndices_rhs[i];
		       }

		      }




	//		 }}


	     vector<int> finalBarCode = {};
	/**     for(unsigned  int i = 0; i < 6; i++)
	     {
	        finalBarCode.push_back(maxProbabilitiesDigits_lhs[i]);
	     }
	     */
	     for(unsigned  int i = 0; i < 6; i++)
	     {
	    	  finalBarCode.push_back(maxProbabilitiesDigits_rhs[i]);
	     }

/**
	     for(unsigned int i = 0; i < 6; i++)
	     {
	        if((maxProbabilitiesIndices_lhs[i])%2 == 0)
	        {
	          parity_lhs.push_back(1);

	        }
	        if((maxProbabilitiesIndices_lhs[i])%2 == 1)
	        {
	        	parity_lhs.push_back(0);

	        }


	     }


	       vector<vector<int>> m = {{1, 1, 1, 1, 1, 1}, {1, 1, 0, 1, 0, 0}, {1, 1, 0, 0, 1, 0}, {1,
	           1, 0, 0, 0, 1}, {1, 0, 1, 1, 0, 0}, {1, 0, 0, 1, 1, 0}, {1, 0,
	          0, 0, 1, 1}, {1, 0, 1, 0, 1, 0}, {1, 0, 1, 0, 0, 1}, {1, 0, 0, 1,
	           0, 1}};

	       int mNumber = -1;

	       for(int i = 0; i < 10; i++)
	       {
	         if(m.at(i).at(0) == parity_lhs.at(0) &&
	            m.at(i).at(1) == parity_lhs.at(1) &&
				 m.at(i).at(2) == parity_lhs.at(2) &&
				 m.at(i).at(3) == parity_lhs.at(3) &&
				 m.at(i).at(4) == parity_lhs.at(4) &&
				 m.at(i).at(5) == parity_lhs.at(5) )
	         {
	        	 mNumber = i ; break;
	         }
	       }
	       finalBarCode.insert(finalBarCode.begin() , mNumber);
*/
string rhs_barcode=to_string(finalBarCode[0])+to_string(finalBarCode[1])+to_string(finalBarCode[2])+to_string(finalBarCode[3])+to_string(finalBarCode[4])+to_string(finalBarCode[5]);

	       printf("\nfinal Bar Code method ");
	       for(unsigned int i=0;i<6;i++)
	        {
	    	   printf(" %d ",finalBarCode[i]);
	        }

	    	//printf(checkSum(finalBarCode) ? "true" : "false");


return rhs_barcode;





}}
}

string barcode_lhs(long double st,vector<vector<int>> bars,long double avbl1,long double avbl0,long double d)
{

	  long double delta = d;
	  long double step = st;

	  long double avgBarLength0 = avbl0;
	  long double  avgBarLength1 = avbl1;
	 vector<vector<int>>  s = bars;
	 vector<vector<vector<int>>>  s_lhs;
//	 vector<vector<vector<int>>>  s_rhs;

	 s_lhs=lhsBars(s);

	// s_rhs=rhsBars(s);



	  vector<long double> maxProbabilities_lhs = {0, 0, 0, 0, 0, 0};
	  vector<int> maxProbabilitiesDigits_lhs = {0, 0, 0, 0, 0, 0};
	  vector<int> maxProbabilitiesIndices_lhs = {0, 0, 0, 0, 0, 0};
	  int maxProbIndex_lhs = 0;
	  long double maxProb_lhs=0;
	  vector<int> maxProbabilitiesDigits2_lhs = {0, 0, 0, 0, 0, 0};
	  vector<int> maxProbabilitiesIndices2_lhs = {0, 0, 0, 0, 0, 0};
	  vector<long double> maxProbabilities2_lhs = {0, 0, 0, 0, 0, 0};
	  vector<long double> finalProbList_lhs = {};
	  vector<double> parity_lhs = {};


/**
	  vector<long double> maxProbabilities_rhs = {0, 0, 0, 0, 0, 0};
	  vector<int> maxProbabilitiesDigits_rhs = {0, 0, 0, 0, 0, 0};
	  vector<int> maxProbabilitiesIndices_rhs = {0, 0, 0, 0, 0, 0};
	  int maxProbIndex_rhs = 0;
	  long double maxProb_rhs=0;
	  vector<int> maxProbabilitiesDigits2_rhs = {0, 0, 0, 0, 0, 0};
	  vector<int> maxProbabilitiesIndices2_rhs = {0, 0, 0, 0, 0, 0};
	  vector<long double> maxProbabilities2_rhs = {0, 0, 0, 0, 0, 0,};
	  vector<long double> finalProbList_rhs = {};
	  vector<double> parity_rhs = {};
	  */
	  vector<int> finalbarCode={};


	  long double sigma = 1;
	  int digitNumber = -1;


	 for(long double c = (avgBarLength1 - delta); c <= (avgBarLength1 + delta); c = c + step)
	  {
	     for(long double e = (avgBarLength0 - delta); e <= (avgBarLength0 + delta); e = e + step)
		{


	      vector<long double> r0L ={c*3, e*2, c, e}; ;
	      vector<long double> r0G = {c, e, c*2, e*3};

	      vector<long double> r1L = {c*2, e*2, c*2, e};
	      vector<long double> r1G = {c, e*2, c*2, e*2};

	      vector<long double> r2L = {c*2, e, c*2, e*2};
	      vector<long double> r2G = {c*2, e*2, c, e*2};

	      vector<long double> r3L = {c, e*3, c, e};
	      vector<long double> r3G = {c, e, c*4, e};

	      vector<long double> r4L = {c, e, c*3, e*2};
	      vector<long double> r4G = {c*2, e*3, c, e};

	      vector<long double> r5L = {c, e*2, c*3, e};
	      vector<long double> r5G = {c, e*3, c*2, e};

	      vector<long double> r6L = {c, e, c, e*4};
	      vector<long double> r6G = {c*4, e, c, e};

	      vector<long double> r7L = {c, e*3, c, e*2};
	      vector<long double> r7G = {c*2, e, c*3, e};

	      vector<long double> r8L = {c, e*2, c, e*3};
	      vector<long double> r8G = {c*3, e, c*2, e};

	      vector<long double> r9L = {c*3, e, c, e*2};
	      vector<long double> r9G = {c*2, e, c, e*3};


	      vector<vector<long double>> r = {r0L, r0G, r1L, r1G,  r2L, r2G,  r3L, r3G,
	        r4L, r4G,  r5L, r5G,  r6L, r6G,  r7L, r7G, r8L,
	         r8G,  r9L, r9G};





	      for(int a = 0; a < 6; a++)
	      {

	       maxProb_lhs = 0;
	       maxProbIndex_lhs = 0;

	       for(int b = 0; b < 20; b++)
	       {

	        if(p_lhs(b, a, c, e, s_lhs) >= maxProb_lhs)
	        {
	        	maxProb_lhs = p_lhs(b, a, c, e, s_lhs);

	            maxProbIndex_lhs = b;
	        }

	       }


	       sigma = sigma*maxProb_lhs;





	       digitNumber = ((maxProbIndex_lhs - maxProbIndex_lhs%2)/2);




	       if(maxProb_lhs >= maxProbabilities_lhs[a])
	       {
	        maxProbabilities_lhs[a] = maxProb_lhs;
	        maxProbabilitiesDigits_lhs[a] = digitNumber;
	        maxProbabilitiesIndices_lhs[a] = maxProbIndex_lhs;
	       }



	       }



	      for(unsigned  int i = 0; i < 6; i++)
	      {
	       if(maxProbabilities_lhs[i] >= maxProbabilities2_lhs[i])
	       {
	        maxProbabilities2_lhs[i] = maxProbabilities_lhs[i];
	        maxProbabilitiesDigits2_lhs[i] = maxProbabilitiesDigits_lhs[i];
	        maxProbabilitiesIndices2_lhs[i] = maxProbabilitiesIndices_lhs[i];
	       }

	      }




		 }}



	 	 	 sigma=1;
	 	 	 /**
	 for(long double c = (avgBarLength1 - delta); c <= (avgBarLength1 + delta); c = c + step)
		  {
		     for(long double e = (avgBarLength0 - delta); e <= (avgBarLength0 + delta); e = e + step)
			{



		      vector<long double> r0R = {e*3, c*2, e, c};

		      vector<long double> r1R = {e*2, c*2, e*2, c};

		      vector<long double> r2R = {e*2, c, e*2, c*2};

		      vector<long double> r3R = {e, c*3, e, c};

		      vector<long double> r4R = {e, c, e*3, c*2};

		      vector<long double> r5R = {e, c*2, e*3, c};

		      vector<long double> r6R = {e, c, e, c*4};

		      vector<long double> r7R = {e, c*3, e, c*2};

		      vector<long double> r8R = {e, c*2, e, c*3};

		      vector<long double> r9R = {e*3, c, e, c*2};

		      vector<vector<long double>> r = { r0R, r1R,  r2R,  r3R,
		         r4R,  r5R,  r6R,  r7R,  r8R,  r9R};





		      for(int a = 0; a < 6; a++)
		      {

		       maxProb_rhs = 0;
		       maxProbIndex_rhs = 0;

		       for(int b = 0; b < 10; b++)
		       {

		        if(p_rhs(b, a, c, e, s_rhs) >= maxProb_rhs)
		        {
		        	maxProb_rhs = p_rhs(b, a, c, e, s_rhs);

		            maxProbIndex_rhs = b;
		        }

		       }


		       sigma = sigma*maxProb_rhs;





		       digitNumber = maxProbIndex_rhs;



		       if(maxProb_rhs >= maxProbabilities_rhs[a])
		       {
		        maxProbabilities_rhs[a] = maxProb_rhs;
		        maxProbabilitiesDigits_rhs[a] = digitNumber;
		        maxProbabilitiesIndices_rhs[a] = maxProbIndex_rhs;
		       }



		       }



		      for(unsigned  int i = 0; i < 6; i++)
		      {
		       if(maxProbabilities_rhs[i] >= maxProbabilities2_rhs[i])
		       {
		        maxProbabilities2_rhs[i] = maxProbabilities_rhs[i];
		        maxProbabilitiesDigits2_rhs[i] = maxProbabilitiesDigits_rhs[i];
		        maxProbabilitiesIndices2_rhs[i] = maxProbabilitiesIndices_rhs[i];
		       }

		      }





*/

	     vector<int> finalBarCode = {};
	     for(unsigned  int i = 0; i < 6; i++)
	     {
	        finalBarCode.push_back(maxProbabilitiesDigits_lhs[i]);
	     }
/**	     for(unsigned  int i = 0; i < 6; i++)
	     {
	    	  finalBarCode.push_back(maxProbabilitiesDigits_rhs[i]);
	     }
*/

	     for(unsigned int i = 0; i < 6; i++)
	     {
	        if((maxProbabilitiesIndices_lhs[i])%2 == 0)
	        {
	          parity_lhs.push_back(1);

	        }
	        if((maxProbabilitiesIndices_lhs[i])%2 == 1)
	        {
	        	parity_lhs.push_back(0);

	        }


	     }


	       vector<vector<int>> m = {{1, 1, 1, 1, 1, 1}, {1, 1, 0, 1, 0, 0}, {1, 1, 0, 0, 1, 0}, {1,
	           1, 0, 0, 0, 1}, {1, 0, 1, 1, 0, 0}, {1, 0, 0, 1, 1, 0}, {1, 0,
	          0, 0, 1, 1}, {1, 0, 1, 0, 1, 0}, {1, 0, 1, 0, 0, 1}, {1, 0, 0, 1,
	           0, 1}};

	       int mNumber = -1;

	       for(int i = 0; i < 10; i++)
	       {
	         if(m.at(i).at(0) == parity_lhs.at(0) &&
	            m.at(i).at(1) == parity_lhs.at(1) &&
				 m.at(i).at(2) == parity_lhs.at(2) &&
				 m.at(i).at(3) == parity_lhs.at(3) &&
				 m.at(i).at(4) == parity_lhs.at(4) &&
				 m.at(i).at(5) == parity_lhs.at(5) )
	         {
	        	 mNumber = i ; break;
	         }
	       }
	       finalBarCode.insert(finalBarCode.begin() , mNumber);

          string lhs_string="";

	       printf("\nfinal Bar Code method %s\n",lhs_string);
	       for(unsigned int i=0;i<7;i++)
	        {
	           lhs_string=lhs_string+to_string(finalBarCode[i]);
	    	   printf(" %d ",finalBarCode[i]);
	        }

            return lhs_string;
//	    	printf(checkSum(finalBarCode) ? "true" : "false");









}
string int_to_string(int i)
{
    switch(i) {
      case 0 :
         return "0";
      case 1 :
          return "1";
      case 2 :
          return "2";
      case 3 :
         return "3";
      case 4 :
         return "4";
      case 5 :
         return "5";
    case 6 :
         return "6";
         case 7 :
         return "7";
         case 8 :
         return "8";
         case 9 :
         return "9";
      default :
         return "";
   }
}
int main(int argc, char *argv[])
{
    bool debug=false;

string file_name=argv[1];



    if(strcmp(argv[2],"true")==0)
    {
        debug=true;
    }


//    string file_name="C:\\Users\\Gerard\\Desktop\\business\\barcodes\\barcode1_mbdb.jpg";
	 vector<long double> scanline=getScanline(file_name,debug);


	 clock_t start = clock();


	 vector<vector<int>> Bars=bars(scanline,debug);


	 vector<int> lPadding=leftPadding(Bars);
	 vector<int> rPadding=rightPadding(Bars);
	 vector<vector<int>> lGuard=leftGuard(Bars);
	 vector<vector<int>> rGuard=rightGuard(Bars);
	 vector<vector<int>> mGuard=midGuard(Bars);
	 vector<vector<vector<int>>> lAndRBars=lhsAndRhsBars(Bars);
	 vector<vector<vector<int>>> lBars=lhsBars(Bars);
	 vector<vector<vector<int>>> rBars=rhsBars(Bars);
	 double avbl1=averageBarLength1(Bars);
	 double avbl0=averageBarLength0(Bars);

	 if(debug)
     {


	 printf("\naverage bar length 0 %f\naverage bar length 1 %f",avbl0,avbl1);

	 printf("\nleft padding %llu right padding %llu",lPadding.size(),rPadding.size());

	 printf("\nleft guard");
	 for(unsigned int i=0;i<lGuard.size();i++)
	 {
		 printf("\n");
		 for(unsigned int j=0;j<lGuard.at(i).size();j++)
		 {
		 printf("%d",lGuard.at(i).at(j));
		 }
	 }
	 printf("\nright guard");
	 	 for(unsigned  int i=0;i<rGuard.size();i++)
	 	 {
	 		 printf("\n");
	 		 for(unsigned int j=0;j<rGuard.at(i).size();j++)
	 		 {
	 		 printf("%d",rGuard.at(i).at(j));
	 		 }
	 	 }
	 	 printf("\nmid guard");
	 		 	 for(unsigned int i=0;i<mGuard.size();i++)
	 		 	 {
	 		 		 printf("\n");
	 		 		 for(unsigned int j=0;j<mGuard.at(i).size();j++)
	 		 		 {
	 		 		 printf("%d",mGuard.at(i).at(j));
	 		 		 }
	 		 	 }




     }

std::future<string> lhsDigits = std::async(&barcode_lhs, 0.1,Bars,avbl1,avbl0,0);
string lhs = lhsDigits.get();
cout<<"\nlhs "<<lhs;

std::future<string> rhsDigits = std::async(&barcode_rhs, 0.1,Bars,avbl1,avbl0,0);
string rhs = rhsDigits.get();
cout<<"\nrhs "<<rhs;



	  clock_t end = clock();
	  double time = (double) (end-start);
	  printf("\ntime %f",time/1000);


;

		      	    return 0;
}

