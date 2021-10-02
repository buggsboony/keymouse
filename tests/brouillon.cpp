

/*calcul de factoriel*/
long long fact(long long n)
{//cas particuliers: 0! et 1!
if((n==0)||(n==1)){return 1;}
return n*fact(n-1);
}

 
// // Utilisation :  get all combinations for modiders masks
//  vector<int> vlist={ ControlMask,ShiftMask,AltMask,NumLockMask};
//  auto combinations = getModifierComb(vlist);
//     for(auto m : combinations)
//     {
//         cout<<m.second<<endl;
//     }

map<int,int> getModifierComb(vector<int> vlist)
{
    map<int,int>results;

    
    // ControlMask=4
    // ShiftMask=1
    // NumLockMask=16
    // AltMask=8
    // ControlMask|ShiftMask=5
    // ControlMask|AltMask=12
    // ControlMask|NumLockMask=20
    // ShiftMask|AltMask=9
    // ShiftMask|NumLockMask=17
    // AltMask|NumLockMask=24
    // ControlMask|ShiftMask|AltMask=13
    // ControlMask|ShiftMask|NumLockMask=21
    // ControlMask|ShiftMask|AltMask|NumLockMask=29
 
    // cout<<"ControlMask="<<ControlMask<<endl;
    // cout<<"ShiftMask="<<ShiftMask<<endl;
    // cout<<"NumLockMask="<<NumLockMask<<endl;
    // cout<<"AltMask="<<AltMask<<endl;

    // cout<<"ControlMask|ShiftMask="<<(ControlMask|ShiftMask)<<endl;
    // cout<<"ControlMask|AltMask="<<(ControlMask|AltMask)<<endl;
    // cout<<"ControlMask|NumLockMask="<<(ControlMask|NumLockMask)<<endl;


    // cout<<"ShiftMask|AltMask="<<(ShiftMask|AltMask)<<endl;
    // cout<<"ShiftMask|NumLockMask="<<(ShiftMask|NumLockMask)<<endl;


    // cout<<"AltMask|NumLockMask="<<(AltMask|NumLockMask)<<endl;


    // cout<<"ControlMask|ShiftMask|AltMask="<<(ControlMask|ShiftMask|AltMask)<<endl;
    // cout<<"ControlMask|ShiftMask|NumLockMask="<<(ControlMask|ShiftMask|NumLockMask)<<endl;
    // cout<<"ControlMask|ShiftMask|AltMask|NumLockMask="<<(ControlMask|ShiftMask|AltMask|NumLockMask)<<endl;

        
    long long n_count = fact(vlist.size() );
    cout<<"ncount="<<n_count<<endl;
    int vmem=0; int ntour=0;
    int result=0, offset=0;
    for(int i=0; i<n_count; i++)
    {
        //cout<<"i="<<i<<endl;
        int m = i % vlist.size();
        int v= vlist[m];
        //cout<<"m="<<m<<" "<<"v="<<v<<endl;
        
        if(m==0) //modulo Nouveau tour
        {        
            //cout<<"vmem+=v["<<ntour<<"] soit "<<vlist[ntour]<<endl;
            
            //cout<<"ntour="<<ntour<<" vmem="<<vmem<<endl;
            ntour++;
            //vmem|=offset; //vmem on previous offset
            offset = vlist[ntour-1];//get equivalent 
            if(ntour>=vlist.size() ) 
            {
                ntour=1;
                vmem|=vlist[ntour-1];
            }
        }else
        {
            
        }
            
        result= vmem|offset | v;
        results[result]=result;//one -> one
        //cout<<"RESULT="<<result<<endl;

    }//next

    //add all 
    result=0;
    for(auto v :vlist)
    {
        result|=v;
    }
    //cout<<"LAST RESULT="<<result<<endl;
    results[result]=result;    
    return results;
}

