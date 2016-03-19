#include <iostream>
using namespace std;

int  main()
{
    int m;
    int r;
    int db[30];
     
    cout<<"\n Enter frame size:";
    cin>>m;
     
    int i,j,k; 
     
    cout<<"\n Enter Frame :";
    for (i=m-1;i>=0;i--)
    {
        cin>>db[i];
    }
 
    cout<<"\n Enter redundancy bits: ";
    cin>>r;
     
    //Entered Frame
    cout<<"\n Frame :\n ";
    for(i=0;i<m;i++)
    {
        cout<<db[i]<<" ";
    }
 
    //Reverse db[] for further processing
    int temp[30];
    for (i=m-1,j=0;i>=0;i--,j++)
    {
        temp[j]=db[i];
    }
    for (i=0;i<m;i++)
    {
        db[i]=temp[i];
    }
 
    int nrb=0;      // Counter for number of redundancy bit
    int rb[8];      // Array to hold redundancy bits
 
    int l=0;        // Length of new frame = l = m+r
    int df[30];     // New frame
 
    // Introduce redundancy bits and construct new frame
    k=1;
    for(i=1,j=0;j<m;i++)
    {
        //If it is location of redundancy bit
        if(i==k)
        {
            df[i]=-1;
            k=k*2;
            nrb++;
        }
        //If it is location of data bit
        else
        {
            df[i]=db[j];
            j++;
        }
        l++;
    }
 
    cout<<"\n";
    cout<<"\n Frame after introducing redundancy bits :\n ";
    for(i=l;i>0;i--)
    {
        cout<<df[i]<<" ";
    }
 
    //Some more temp variables and arrays
    int i1,i2,i3,i4,i5,i6,i7;
    int a1[4],a2[4];
     
    i5=0;       //Holds position of rb[]
 
    //Scan Whole frame and calculate all redundancy bits
    for(i1=1;i1<=l;i1++)
    {
        //check if it is redundancy bit which is to be calculated
        if(df[i1]==-1)
        {
            //Get the position of 1 from binary representation
            i4 = -1;    //Hold position of 1
             
            //Converting redundancy bit position into binary form and get position of 1
            i7=i1;
            while (i7>0)
            {
                i4++;
                if(i7==1)
                {
                    break;
                }
                else
                {
                    i7=i7/2;
                }
            }
 
            i3=0; // Consider even parity
 
            //Calculating redundancy bit and parity
            for(i2=1;i2<=l;i2++)
            {
                //reset a1 frame
                for(i6=0;i6<4;i6++)
                {
                    a1[i6]=-1;
                }
 
                //find binary conversion for each position
                i7=i2;
                i6=0;
                while(i7>0)
                {
                    if(i7==1)
                    {
                        a1[i6]=1;
                        break;
                    }
                    else if(i7==0)
                    {
                        a1[i6]=0;
                    }
                    else
                    {
                        a1[i6]=i7%2;
                        i7=i7/2;
                    }
                    i6++;
                }
 
                //check that the binary conversion is having bit at specific position or not
                //if yes calculate parity
                 
                if(a1[i4]==1)
                {
                    if(df[i2]==1)
                    {
                        i3++;
                    }
                }
                 
            }
 
            //Display Parity
            //Add redundancy bit in data frame
            //odd parity -> 1 :: even parity -> 0
 
            if(i3%2==0)
            {
                df[i1]=0;
                rb[i5]=0;
            }
            else
            {
                df[i1]=1;
                rb[i5]=1;
            }
            i5++;
 
            //Print current frame
            cout<<"\n R"<<i5<<" = "<<rb[i5-1]<<"\t New Frame";
            for(i2=1;i2<=l;i2++)
            {
                cout<<" "<<df[i2];
            }
        }
     
    }
 
    //Get the error bit
    int eb;
    cout<<"\n";
    cout<<"\n Enter bit number where error is occurred: ";
    cin>>eb;
     
    cout<<"\n Bit at position "<<eb<<" is "<<df[eb];
     
    //Change bit
    if(df[eb]=1)
    {
        df[eb]=0;
    }
    else
    {
        df[eb]=1;
    }
     
    cout<<" and now changed to "<<df[eb];
     
    // Print new frame after changing the bit
    cout<<"\n New Frame is";
    for(i=l;i>0;i--)
    {
        cout<<" "<<df[i];
    }
 
    //Recalculate everything
 
    i5=0;
    k=1;
    //Scan Whole frame
    for(i1=1;i1<=l;i1++)
    {       
        //Do processing only for redundancy bits
        if (i1==k)
        {
            //Get the position of 1 from binary representation
            i4 = -1; //Hold position of 1
         
            //Converting redundancy bit into binary form and get position of 1
            i7=i1;
            while (i7>0)
            {
                i4++;
                if(i7==1)
                {
                    break;
                }
                else
                {
                    i7=i7/2;
                }
            }
 
            i3=0; // Consider even parity
         
            //Calculating redundancy bit and parity
            for(i2=1;i2<=l;i2++)
            {
                //reset a1 frame
                for(i6=0;i6<4;i6++)
                {
                    a1[i6]=-1;
                }
             
                //find binary conversion
                i7=i2;
                i6=0;
                while(i7>0)
                {
                    if(i7==1)
                    {
                        a1[i6]=1;
                        break;
                    }
                    else if(i7==0)
                    {
                        a1[i6]=0;
                    }
                    else
                    {
                        a1[i6]=i7%2;
                        i7=i7/2;
                    }
                    i6++;
                }
             
                //check that the binary conversion is having bit at specific position or not
                //if yes calculate parity
             
                if(a1[i4]==1)
                {
                    if(df[i2]==1)
                    {
                        i3++;
                    }
                }
                 
            }
 
            //Display Parity
            //Add redundancy bit in data frame
            //odd parity -> 1 :: even parity -> 0
         
            if(i3%2==0)
            {
                df[i1]=0;
                rb[i5]=0;
            }
            else
            {
                df[i1]=1;
                rb[i5]=1;
            }
         
            i5++;
         
            k=k*2;
        }
 
    }
 
    cout<<"\n Redundancy bits:";
    for(i=0;i<nrb;i++)
    {
        cout<<" "<<rb[i];
    }
 
    //Calculate error position
    int ep=0;
    for(i=0,j=1;i<nrb;i++)
    {
        if(rb[i]==1)
        {
            ep=ep+j;
        }
        j=j*2;
    }
 
    cout<<"\n Error is at : "<<ep;
 
}