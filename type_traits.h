#ifndef TYPE_TRAITS_H_INCLUDED
#define TYPE_TRAITS_H_INCLUDED

namespace TinySTL
{
    struct _true_type {};
    struct _false_type {};

    template<class type>
    struct _type_traits{
        using this_dummy_member_must_be_first   =_true_type;

        using has_trivial_default_constructor   =_false_type;
        using has_trivial_copy_constructor      =_false_type;
        using has_trivial_assigngnment_operator =_false_type;
        using has_trivial_destructor            =_false_type;
        using is_POD_type                       =_false_type;
    };

    template<>
    struct _type_traits<bool>{
        using has_trivial_default_constructor   =_true_type;
        using has_trivial_copy_constructor      =_true_type;
        using has_trivial_assigngnment_operator =_true_type;
        using has_trivial_destructor            =_true_type;
        using is_POD_type                       =_true_type;
    };

    template<>
    struct _type_traits<wchar_t>{
        using has_trivial_default_constructor   =_true_type;
        using has_trivial_copy_constructor      =_true_type;
        using has_trivial_assigngnment_operator =_true_type;
        using has_trivial_destructor            =_true_type;
        using is_POD_type                       =_true_type;
    };

    template<>
    struct _type_traits<char>{
        using has_trivial_default_constructor   =_true_type;
        using has_trivial_copy_constructor      =_true_type;
        using has_trivial_assigngnment_operator =_true_type;
        using has_trivial_destructor            =_true_type;
        using is_POD_type                       =_true_type;
    };

    template<>
    struct _type_traits<signed char>{
        using has_trivial_default_constructor   =_true_type;
        using has_trivial_copy_constructor      =_true_type;
        using has_trivial_assigngnment_operator =_true_type;
        using has_trivial_destructor            =_true_type;
        using is_POD_type                       =_true_type;
    };

    template<>
    struct _type_traits<unsigned char>{
        using has_trivial_default_constructor   =_true_type;
        using has_trivial_copy_constructor      =_true_type;
        using has_trivial_assigngnment_operator =_true_type;
        using has_trivial_destructor            =_true_type;
        using is_POD_type                       =_true_type;
    };

    template<>
    struct _type_traits<short>{
        using has_trivial_default_constructor   =_true_type;
        using has_trivial_copy_constructor      =_true_type;
        using has_trivial_assigngnment_operator =_true_type;
        using has_trivial_destructor            =_true_type;
        using is_POD_type                       =_true_type;
    };

    template<>
    struct _type_traits<unsigned short>{
        using has_trivial_default_constructor   =_true_type;
        using has_trivial_copy_constructor      =_true_type;
        using has_trivial_assigngnment_operator =_true_type;
        using has_trivial_destructor            =_true_type;
        using is_POD_type                       =_true_type;
    };

    template<>
    struct _type_traits<int>{
        using has_trivial_default_constructor   =_true_type;
        using has_trivial_copy_constructor      =_true_type;
        using has_trivial_assigngnment_operator =_true_type;
        using has_trivial_destructor            =_true_type;
        using is_POD_type                       =_true_type;
    };

    template<>
    struct _type_traits<unsigned int>{
        using has_trivial_default_constructor   =_true_type;
        using has_trivial_copy_constructor      =_true_type;
        using has_trivial_assigngnment_operator =_true_type;
        using has_trivial_destructor            =_true_type;
        using is_POD_type                       =_true_type;
    };

    template<>
    struct _type_traits<long>{
        using has_trivial_default_constructor   =_true_type;
        using has_trivial_copy_constructor      =_true_type;
        using has_trivial_assigngnment_operator =_true_type;
        using has_trivial_destructor            =_true_type;
        using is_POD_type                       =_true_type;
    };

    template<>
    struct _type_traits<long long>{
        using has_trivial_default_constructor   =_true_type;
        using has_trivial_copy_constructor      =_true_type;
        using has_trivial_assigngnment_operator =_true_type;
        using has_trivial_destructor            =_true_type;
        using is_POD_type                       =_true_type;
    };

    template<>
    struct _type_traits<unsigned long long>{
        using has_trivial_default_constructor   =_true_type;
        using has_trivial_copy_constructor      =_true_type;
        using has_trivial_assigngnment_operator =_true_type;
        using has_trivial_destructor            =_true_type;
        using is_POD_type                       =_true_type;
    };

    template<>
    struct _type_traits<unsigned long>{
        using has_trivial_default_constructor   =_true_type;
        using has_trivial_copy_constructor      =_true_type;
        using has_trivial_assigngnment_operator =_true_type;
        using has_trivial_destructor            =_true_type;
        using is_POD_type                       =_true_type;
    };

    template<>
    struct _type_traits<float>{
        using has_trivial_default_constructor   =_true_type;
        using has_trivial_copy_constructor      =_true_type;
        using has_trivial_assigngnment_operator =_true_type;
        using has_trivial_destructor            =_true_type;
        using is_POD_type                       =_true_type;
    };

    template<>
    struct _type_traits<double>{
        using has_trivial_default_constructor   =_true_type;
        using has_trivial_copy_constructor      =_true_type;
        using has_trivial_assigngnment_operator =_true_type;
        using has_trivial_destructor            =_true_type;
        using is_POD_type                       =_true_type;
    };

    template<>
    struct _type_traits<long double>{
        using has_trivial_default_constructor   =_true_type;
        using has_trivial_copy_constructor      =_true_type;
        using has_trivial_assigngnment_operator =_true_type;
        using has_trivial_destructor            =_true_type;
        using is_POD_type                       =_true_type;
    };

    template<class T>
    struct _type_traits<T*>{
        using has_trivial_default_constructor   =_true_type;
        using has_trivial_copy_constructor      =_true_type;
        using has_trivial_assigngnment_operator =_true_type;
        using has_trivial_destructor            =_true_type;
        using is_POD_type                       =_true_type;
    };

    template<class T>
    struct _type_traits<const T*>{
        using has_trivial_default_constructor   =_true_type;
        using has_trivial_copy_constructor      =_true_type;
        using has_trivial_assigngnment_operator =_true_type;
        using has_trivial_destructor            =_true_type;
        using is_POD_type                       =_true_type;
    };

    template<>
    struct _type_traits<const char*>{
        using has_trivial_default_constructor   =_true_type;
        using has_trivial_copy_constructor      =_true_type;
        using has_trivial_assigngnment_operator =_true_type;
        using has_trivial_destructor            =_true_type;
        using is_POD_type                       =_true_type;
    };

    template<>
    struct _type_traits<char*>{
        using has_trivial_default_constructor   =_true_type;
        using has_trivial_copy_constructor      =_true_type;
        using has_trivial_assigngnment_operator =_true_type;
        using has_trivial_destructor            =_true_type;
        using is_POD_type                      =_true_type;
    };

    template<>
    struct _type_traits<const unsigned char*>{
        using has_trivial_default_constructor   =_true_type;
        using has_trivial_copy_constructor      =_true_type;
        using has_trivial_assigngnment_operator =_true_type;
        using has_trivial_destructor            =_true_type;
        using is_POD_type                       =_true_type;
    };

    template<>
    struct _type_traits<unsigned char*>{
        using has_trivial_default_constructor   =_true_type;
        using has_trivial_copy_constructor      =_true_type;
        using has_trivial_assigngnment_operator =_true_type;
        using has_trivial_destructor            =_true_type;
        using is_POD_type                       =_true_type;
    };

    template<>
    struct _type_traits<signed char*>{
        using has_trivial_default_constructor   =_true_type;
        using has_trivial_copy_constructor      =_true_type;
        using has_trivial_assigngnment_operator =_true_type;
        using has_trivial_destructor            =_true_type;
        using is_POD_type                       =_true_type;
    };

    template<>
    struct _type_traits<const signed char*>{
        using has_trivial_default_constructor   =_true_type;
        using has_trivial_copy_constructor      =_true_type;
        using has_trivial_assigngnment_operator =_true_type;
        using has_trivial_destructor            =_true_type;
        using is_POD_type                       =_true_type;
    };
}

#endif // TYPE_TRAITS_H_INCLUDED
